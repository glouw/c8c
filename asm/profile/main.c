--------------------------------------------------------------------------------
Profile data file 'callgrind.out.25895' (creator: callgrind-3.11.0)
--------------------------------------------------------------------------------
I1 cache: 
D1 cache: 
LL cache: 
Timerange: Basic block 0 - 72407
Trigger: Program termination
Profiled target:  ./asm MAIN.asm MAIN.hex (PID 25895, part 1)
Events recorded:  Ir
Events shown:     Ir
Event sort order: Ir
Thresholds:       99
Include dirs:     
User annotated:   main.c
Auto-annotation:  off

--------------------------------------------------------------------------------
     Ir 
--------------------------------------------------------------------------------
286,407  PROGRAM TOTALS

--------------------------------------------------------------------------------
    Ir  file:function
--------------------------------------------------------------------------------
62,907  ???:_dl_addr [/usr/lib/libc-2.22.so]
30,717  ???:do_lookup_x [/usr/lib/ld-2.22.so]
27,301  ???:strtok [/usr/lib/libc-2.22.so]
22,948  ???:_dl_lookup_symbol_x [/usr/lib/ld-2.22.so]
21,748  ???:_dl_relocate_object [/usr/lib/ld-2.22.so]
12,835  ???:strcmp [/usr/lib/libc-2.22.so]
12,280  ???:__GI_memchr [/usr/lib/libc-2.22.so]
11,790  ???:vfprintf [/usr/lib/libc-2.22.so]
 9,078  ???:getdelim [/usr/lib/libc-2.22.so]
 6,898  ???:check_match [/usr/lib/ld-2.22.so]
 4,499  ???:_IO_file_xsputn@@GLIBC_2.1 [/usr/lib/libc-2.22.so]
 4,170  ???:__strchr_sse2_bsf [/usr/lib/libc-2.22.so]
 3,029  main.c:main [/home/gl/Projects/chip8/asm/asm]
 2,742  ???:__GI_memcpy [/usr/lib/libc-2.22.so]
 2,315  ???:__GI_strcmp [/usr/lib/libc-2.22.so]
 2,207  ???:_dl_name_match_p [/usr/lib/ld-2.22.so]
 2,129  ???:strchrnul [/usr/lib/libc-2.22.so]
 2,019  ???:intel_check_word [/usr/lib/libc-2.22.so]
 1,843  ???:_dl_map_object_from_fd [/usr/lib/ld-2.22.so]
 1,768  ???:fputc [/usr/lib/libc-2.22.so]
 1,725  /usr/include/bits/stdlib-bsearch.h:assemble
 1,722  ???:_dl_check_map_versions [/usr/lib/ld-2.22.so]
 1,553  ???:_dl_important_hwcaps [/usr/lib/ld-2.22.so]
 1,441  ???:_dl_fixup [/usr/lib/ld-2.22.so]
 1,302  assemble.c:assemble [/home/gl/Projects/chip8/asm/asm]
 1,194  ???:_dl_map_object_deps [/usr/lib/ld-2.22.so]
 1,156  ???:dl_main [/usr/lib/ld-2.22.so]
 1,147  ???:__GI_mempcpy [/usr/lib/libc-2.22.so]
 1,114  ???:_dl_cache_libcmp [/usr/lib/ld-2.22.so]
 1,047  assemble.c:ld [/home/gl/Projects/chip8/asm/asm]
 1,017  ???:_int_malloc [/usr/lib/libc-2.22.so]
   952  assemble.c:compare [/home/gl/Projects/chip8/asm/asm]
   936  ???:getline [/usr/lib/libc-2.22.so]
   908  ???:strsep [/usr/lib/ld-2.22.so]
   834  ???:strlen [/usr/lib/ld-2.22.so]
   796  ???:0x00018efc [/usr/lib/ld-2.22.so]
   668  ???:0x00122d55 [/usr/lib/libc-2.22.so]
   657  ???:_int_free [/usr/lib/libc-2.22.so]
   655  ???:malloc_init_state [/usr/lib/libc-2.22.so]
   648  ???:_dl_start [/usr/lib/ld-2.22.so]
   639  ???:_dl_new_object [/usr/lib/ld-2.22.so]
   610  ???:index [/usr/lib/libc-2.22.so]
   595  ???:__libc_memalign [/usr/lib/ld-2.22.so]
   539  ???:malloc [/usr/lib/libc-2.22.so]
   476  ???:match_symbol [/usr/lib/ld-2.22.so]
   469  ???:_dl_load_cache_lookup [/usr/lib/ld-2.22.so]
   460  ???:_dl_sysdep_start [/usr/lib/ld-2.22.so]
   428  ???:_dl_map_object [/usr/lib/ld-2.22.so]
   420  ???:open_verify [/usr/lib/ld-2.22.so]
   407  ???:bcmp [/usr/lib/ld-2.22.so]
   387  ???:memcpy [/usr/lib/ld-2.22.so]
   352  ???:0x00078de0 [/usr/lib/libc-2.22.so]
   338  ???:_dl_allocate_tls_storage [/usr/lib/ld-2.22.so]
   329  ???:_itoa_word [/usr/lib/libc-2.22.so]
   309  ???:_dl_fini [/usr/lib/ld-2.22.so]
   297  ???:fprintf [/usr/lib/libc-2.22.so]
   278  ???:0x00122d59 [/usr/lib/libc-2.22.so]
   269  ???:memset [/usr/lib/ld-2.22.so]
   258  ???:0x00018f04 [/usr/lib/ld-2.22.so]
   253  ???:mmap [/usr/lib/libc-2.22.so]
   239  ???:_IO_file_underflow@@GLIBC_2.1 [/usr/lib/libc-2.22.so]
   238  assemble.c:execute [/home/gl/Projects/chip8/asm/asm]
   237  ???:ptmalloc_init.part.7 [/usr/lib/libc-2.22.so]
   236  assemble.c:add.lto_priv.1 [/home/gl/Projects/chip8/asm/asm]
   222  ???:handle_intel [/usr/lib/libc-2.22.so]
   219  ???:_IO_file_overflow@@GLIBC_2.1 [/usr/lib/libc-2.22.so]
   209  ???:_dl_sort_fini [/usr/lib/ld-2.22.so]
   205  ???:__ctype_b_loc [/usr/lib/libc-2.22.so]
   200  ???:_IO_link_in [/usr/lib/libc-2.22.so]
   194  ???:call_init.part.0 [/usr/lib/ld-2.22.so]
   194  ???:_IO_file_doallocate [/usr/lib/libc-2.22.so]
   187  ???:_dl_runtime_resolve [/usr/lib/ld-2.22.so]
   184  ???:__GI__IO_un_link.part.1 [/usr/lib/libc-2.22.so]
   176  ???:calloc [/usr/lib/ld-2.22.so]
   175  assemble.c:se [/home/gl/Projects/chip8/asm/asm]
   173  ???:sysmalloc [/usr/lib/libc-2.22.so]
   172  ???:fclose@@GLIBC_2.1 [/usr/lib/libc-2.22.so]
   171  assemble.c:sne [/home/gl/Projects/chip8/asm/asm]
   170  ???:__underflow [/usr/lib/libc-2.22.so]
   166  ???:_IO_setb [/usr/lib/libc-2.22.so]
   166  ???:_dl_next_ld_env_entry [/usr/lib/ld-2.22.so]
   154  ???:free [/usr/lib/libc-2.22.so]
   146  ???:fwrite [/usr/lib/libc-2.22.so]
   145  ???:_dl_add_to_namespace_list [/usr/lib/ld-2.22.so]
   141  ???:_dl_catch_error [/usr/lib/ld-2.22.so]
   139  ???:mempcpy [/usr/lib/ld-2.22.so]
   136  ???:_dl_setup_hash [/usr/lib/ld-2.22.so]
   134  ???:new_do_write [/usr/lib/libc-2.22.so]
   129  ???:_IO_file_write@@GLIBC_2.1 [/usr/lib/libc-2.22.so]
   127  ???:_IO_file_fopen@@GLIBC_2.1 [/usr/lib/libc-2.22.so]
   124  ???:_IO_file_close_it@@GLIBC_2.1 [/usr/lib/libc-2.22.so]
   120  ???:_init [/usr/lib/libc-2.22.so]
   119  ???:read_int [/usr/lib/libc-2.22.so]
   118  ???:_IO_file_seekoff@@GLIBC_2.1 [/usr/lib/libc-2.22.so]
   115  ???:_dl_allocate_tls_init [/usr/lib/ld-2.22.so]
   111  ???:_dl_init_paths [/usr/lib/ld-2.22.so]
   104  ???:strdup [/usr/lib/libc-2.22.so]
   100  tree.c:get [/home/gl/Projects/chip8/asm/asm]
    99  assemble.c:drw [/home/gl/Projects/chip8/asm/asm]
    98  ???:_IO_file_open [/usr/lib/libc-2.22.so]
    98  ???:_IO_cleanup [/usr/lib/libc-2.22.so]
    97  ???:_IO_default_xsputn [/usr/lib/libc-2.22.so]
    96  assemble.c:jp [/home/gl/Projects/chip8/asm/asm]
    92  ???:_dl_init [/usr/lib/ld-2.22.so]
    90  ???:__fopen_internal [/usr/lib/libc-2.22.so]
    89  ???:init_tls [/usr/lib/ld-2.22.so]
    88  ???:_IO_switch_to_get_mode [/usr/lib/libc-2.22.so]
    87  ???:_IO_flush_all_lockp [/usr/lib/libc-2.22.so]
    87  ???:_dl_determine_tlsoffset [/usr/lib/ld-2.22.so]
    85  ???:sbrk [/usr/lib/libc-2.22.so]
    81  ???:_dl_discover_osversion [/usr/lib/ld-2.22.so]
    81  assemble.c:rnd [/home/gl/Projects/chip8/asm/asm]
    77  ???:_dl_check_all_versions [/usr/lib/ld-2.22.so]
    77  ???:init_cacheinfo [/usr/lib/libc-2.22.so]
    76  ???:_IO_padn [/usr/lib/libc-2.22.so]
    76  ???:_IO_no_init [/usr/lib/libc-2.22.so]
    72  ???:(below main) [/usr/lib/libc-2.22.so]
    69  ???:_IO_do_write@@GLIBC_2.1 [/usr/lib/libc-2.22.so]
    66  ???:_dl_protect_relro [/usr/lib/ld-2.22.so]
    64  ???:__run_exit_handlers [/usr/lib/libc-2.22.so]
    64  ???:_IO_file_read [/usr/lib/libc-2.22.so]
    62  ???:__init_cpu_features [/usr/lib/libc-2.22.so]
    61  assemble.c:sub [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:subn [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:or [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:shl [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:shr [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:xor [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:and [/home/gl/Projects/chip8/asm/asm]
    59  ???:_dl_debug_initialize [/usr/lib/ld-2.22.so]

--------------------------------------------------------------------------------
-- User-annotated source: main.c
--------------------------------------------------------------------------------
 Ir 

-- line 2 ----------------------------------------
  .  #include <string.h>
  .  #include <stdlib.h>
  .  #include <ctype.h>
  .  #include "tree.h"
  .  #include "assemble.h"
  .  #include <assert.h>
  .  
  .  int main(int argc, char* argv[])
 12  {
  .      /* command line requires an assembly (assem) input and a hexidecimal output (hexid) */
  2      if(argc != 3) return 1;
 11      FILE* assem = fopen(argv[1], "r");
64,836  => ???:fopen@@GLIBC_2.1 (1x)
802  => ???:_dl_runtime_resolve (1x)
  3      if(assem == NULL) return 1;
  6      FILE* hexid = fopen(argv[2], "w");
602  => ???:fopen@@GLIBC_2.1 (1x)
  .      /* FIRST SCAN: gather labels, store in "labels" binary tree (see tree.c) */
  2      tnode* labels = NULL;
  .      /* nlabels keeps track of label count, but is only used to check if 0 for tree init */
  1      unsigned nlabels = 0;
  .      /* 0x0200 reserved for reset vector */
  1      unsigned address = 0x0202;
  .      /* get one line from assem */
  3      char* line = NULL; unsigned len = 0;
433      while(getline(&line, &len, assem) != -1)
13,323  => ???:getline (39x)
878  => ???:_dl_runtime_resolve (1x)
  .      {
  .          /* labels end with a colon - if a colon is found, put address & label pair in tree */
270          char* colon = index(line, ':');
2,083  => ???:__strchr_sse2_bsf (38x)
827  => ???:_dl_runtime_resolve (1x)
 76          if(colon)
  .          {
  2              *colon = '\0';
 33              labels = tree.add(nlabels++ ? labels : NULL, tree.new(line, address));
2,731  => /home/gl/Projects/chip8/asm/tree.c:new (2x)
907  => /home/gl/Projects/chip8/asm/tree.c:add.lto_priv.0 (2x)
  .          }
  .          /* increment address if a mnemonic is found */
310          char* mnemonic = strtok(colon ? colon + 1 : line, "\t \n;");
5,954  => ???:strtok (38x)
833  => ???:_dl_runtime_resolve (1x)
148          if(mnemonic) address += 0x0002;
  .      }
  .      /* if an error occurs during assembly then fail goes high */
  1      unsigned error = 0, fail = 0;
  .      /* write reset vector to hexid if entry name is found */
  .      char* ename = "MAIN";
  5      tnode* enode = tree.get(labels, ename);
 59  => /home/gl/Projects/chip8/asm/tree.c:get (1x)
  2      if(enode == NULL)
  .      {
  .          fail = 1;
  .          fprintf(stderr, "error: %s undefined reference\n", ename);
  .      }
 11      else fprintf(hexid, "1%03X\n", enode->address);
832  => ???:_dl_runtime_resolve (1x)
959  => ???:fprintf (1x)
  .      /* SECOND SCAN: rewind, and generate hex code */
  9      rewind(assem);
863  => ???:_dl_runtime_resolve (1x)
263  => ???:rewind (1x)
  2      unsigned linenumber = 0;
  .      /* get a line from assem */
429      while(getline(&line, &len, assem) != -1)
12,962  => ???:getline (39x)
  .      {
 38          ++linenumber;
  .          /* get mnemonic and operand - skip label if colon is present */
266          char* colon = index(line, ':');
2,087  => ???:__strchr_sse2_bsf (38x)
306          char* mnemonic = strtok(colon ? colon + 1 : line, "\t ;\n");
5,954  => ???:strtok (38x)
190          char* operand = strtok(NULL, ";\n");
7,030  => ???:strtok (38x)
  .          /* if mnemonic was found then assemble (see assemble.c) */
318          if(mnemonic) error = assemble(mnemonic, operand, labels, hexid);
40,325  => /home/gl/Projects/chip8/asm/assemble.c:assemble (34x)
  .          /* otherwise continue onto the next line */
  .          else continue;
  .          /* if an error occurs during assemble (assemble() returns 1,2,3 and so forth) */
102          if(error)
  .          {
  .              /* ...then fail */
  .              fail = 1;
  .              const char* message = "unknown error code";
  .              /* and report the nature of the error */
  .              switch(error)
  .              {
  .                  case 1: message = "unrecognizable operands"; break;
-- line 72 ----------------------------------------
-- line 73 ----------------------------------------
  .                  case 2: message = "label not found"; break;
  .                  case 3: message = "unsupported chip8 mnemonic"; break;
  .                  case 4: message = "missing operand"; break;
  .              }
  .              fprintf(stderr, "error: line %d: %s\n", linenumber, message);
  .          }
  .      }
  .      /* clean up */
  8      free(line);
795  => ???:_dl_runtime_resolve (1x)
137  => ???:free (1x)
  3      tree.print(labels);
3,939  => /home/gl/Projects/chip8/asm/tree.c:print (1x)
  2      tree.delete(labels);
472  => /home/gl/Projects/chip8/asm/tree.c:delete (1x)
  7      fclose(assem);
491  => ???:fclose@@GLIBC_2.1 (1x)
833  => ???:_dl_runtime_resolve (1x)
  4      fclose(hexid);
643  => ???:fclose@@GLIBC_2.1 (1x)
  .      /* remove hexid if complilation failed */
  3      if(fail) remove(argv[2]);
  .      /* go home */
  2      return fail;
  8  }

--------------------------------------------------------------------------------
Ir 
--------------------------------------------------------------------------------
 1  percentage of events annotated

