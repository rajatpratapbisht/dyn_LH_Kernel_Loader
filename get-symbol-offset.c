#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h> // for PATH_MAX
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <elf.h>

static int readElfSection(int , int , const Elf64_Ehdr* ,
                          Elf64_Shdr* , char **);

off_t get_symbol_offset(char *pathname, char *symbol) {
  unsigned char e_ident[EI_NIDENT];
  int rc;
  int symtab_found = 0;
  int foundDebugLib = 0;
  char debugLibName[PATH_MAX] = {0};
  char *shsectData = NULL;

  // FIXME:  Rohan's version wraps the rest of this function
  //         in 'while (retries < 2) { ... }' and then uses two
  //         or three tries to find the correct SHT_STRTAB (namely, ".strtab").

  int fd = open(pathname, O_RDONLY);
  if (fd == -1) {
    return 0; // 0 means pathname not found
  }

  rc = read(fd, e_ident, sizeof(e_ident));
  assert(rc == sizeof(e_ident));
  assert(strncmp(e_ident, ELFMAG, sizeof(ELFMAG)-1) == 0);
  // FIXME:  Add support for 32-bit ELF later
  assert(e_ident[EI_CLASS] == ELFCLASS64);

  // Reset fd to beginning and parse file header
  lseek(fd, 0, SEEK_SET);
  Elf64_Ehdr elf_hdr;
  rc = read(fd, &elf_hdr, sizeof(elf_hdr));
  assert(rc == sizeof(elf_hdr));

  // Get start of symbol table and string table
  Elf64_Off shoff = elf_hdr.e_shoff;
  Elf64_Shdr sect_hdr;
  Elf64_Shdr symtab;
  Elf64_Sym symtab_entry;
  char strtab[10000];
  int i;
  // First, read the data from the shstrtab section
  // This section contains the strings corresponding to the section names
  rc = readElfSection(fd, elf_hdr.e_shstrndx,
                         &elf_hdr, &sect_hdr, &shsectData);
  lseek(fd, shoff, SEEK_SET);
  for (i = 0; i < elf_hdr.e_shnum; i++) {
    rc = read(fd, &sect_hdr, sizeof sect_hdr);
    assert(rc == sizeof(sect_hdr));
    if (sect_hdr.sh_type == SHT_SYMTAB) {
      symtab = sect_hdr;
      symtab_found = 1;
    } else if (sect_hdr.sh_type == SHT_STRTAB &&
               !strcmp(&shsectData[sect_hdr.sh_name], ".strtab")) {
      // Note that there are generally three STRTAB sections in ELF binaries:
      // .dynstr, .shstrtab, and .strtab; We only care about the strtab section.
      int fd2 = open(pathname, O_RDONLY);
      lseek(fd2, sect_hdr.sh_offset, SEEK_SET);
      assert(sect_hdr.sh_size < sizeof(strtab));
      rc = read(fd2, strtab, sect_hdr.sh_size);
      assert(rc == sect_hdr.sh_size);
      close(fd2);
    } else if (sect_hdr.sh_type == SHT_PROGBITS &&
               !strcmp(&shsectData[sect_hdr.sh_name], ".gnu_debuglink")) {
      // If it's the ".gnu_debuglink" section, we read it to figure out
      // the path to the debug symbol file
      Elf64_Shdr tmp;
      char *debugName = NULL;
      // FIXME:  Either expand this inline (if short), or more likely
      //         use readElfSection() also in case of ".strtab", above
      //         (like Rohan's version).
      rc = readElfSection(fd, i, &elf_hdr, &tmp, &debugName);
      assert(debugName);
      // FIXME:  Should find the path, "/usr/lib/debug/lib/x86_64-linux-gnu",
      //         at runtime.
      snprintf(debugLibName, sizeof debugLibName, "%s/%s",
               "/usr/lib/debug/lib/x86_64-linux-gnu", debugName);
      free(debugName);
      foundDebugLib = 1;
    }
  }
  if (! symtab_found) return 0; // No symbol table found.

  // Move to beginning of symbol table
  lseek(fd, symtab.sh_offset, SEEK_SET);
  for ( ; lseek(fd, 0, SEEK_CUR) - symtab.sh_offset < symtab.sh_size ; ) {
    rc = read(fd, &symtab_entry, sizeof symtab_entry);
    assert(rc == sizeof(symtab_entry));
    if (strcmp(strtab + symtab_entry.st_name, symbol) == 0) {
      // found address as offset from base address
      return symtab_entry.st_value;
    }
  }

  close(fd);
}


static int
readElfSection(int fd, int sidx, const Elf64_Ehdr *ehdr,
               Elf64_Shdr *shdr, char **data)
{
  off_t currOff = lseek(fd, 0, SEEK_CUR);
  off_t sidx_off = ehdr->e_shentsize * sidx + ehdr->e_shoff;
  lseek(fd, sidx_off, SEEK_SET);
  int rc = read(fd, shdr, sizeof *shdr);
  assert(rc == sizeof *shdr);
  rc = lseek(fd, shdr->sh_offset, SEEK_SET);
  if (rc > 0) {
    *data = malloc(shdr->sh_size);
    rc = lseek(fd, shdr->sh_offset, SEEK_SET);
    rc = read(fd, *data, shdr->sh_size);
    assert(rc == shdr->sh_size);
  }
  lseek(fd, currOff, SEEK_SET);
  return *data != NULL ? 0 : -1;
}

#ifdef STANDALONE
int main() {
  off_t mmap_offset = get_symbol_offset("/lib64/ld-linux-x86-64.so.2", "mmap");
  if (! mmap_offset) mmap_offset =
         get_symbol_offset("/usr/lib/debug/lib64/ld-linux-x86-64.so.2", "mmap");
  fprintf(stderr, "mmap offset: %p\n", mmap_offset);
  fprintf(stderr, " (Add this to base_address of first LOAD segm. for addr)\n");
  return 0;
}
#endif
