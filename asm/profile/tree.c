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
User annotated:   tree.c
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
   194  ???:_IO_file_doallocate [/usr/lib/libc-2.22.so]
   194  ???:call_init.part.0 [/usr/lib/ld-2.22.so]
   187  ???:_dl_runtime_resolve [/usr/lib/ld-2.22.so]
   184  ???:__GI__IO_un_link.part.1 [/usr/lib/libc-2.22.so]
   176  ???:calloc [/usr/lib/ld-2.22.so]
   175  assemble.c:se [/home/gl/Projects/chip8/asm/asm]
   173  ???:sysmalloc [/usr/lib/libc-2.22.so]
   172  ???:fclose@@GLIBC_2.1 [/usr/lib/libc-2.22.so]
   171  assemble.c:sne [/home/gl/Projects/chip8/asm/asm]
   170  ???:__underflow [/usr/lib/libc-2.22.so]
   166  ???:_dl_next_ld_env_entry [/usr/lib/ld-2.22.so]
   166  ???:_IO_setb [/usr/lib/libc-2.22.so]
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
    87  ???:_dl_determine_tlsoffset [/usr/lib/ld-2.22.so]
    87  ???:_IO_flush_all_lockp [/usr/lib/libc-2.22.so]
    85  ???:sbrk [/usr/lib/libc-2.22.so]
    81  assemble.c:rnd [/home/gl/Projects/chip8/asm/asm]
    81  ???:_dl_discover_osversion [/usr/lib/ld-2.22.so]
    77  ???:_dl_check_all_versions [/usr/lib/ld-2.22.so]
    77  ???:init_cacheinfo [/usr/lib/libc-2.22.so]
    76  ???:_IO_padn [/usr/lib/libc-2.22.so]
    76  ???:_IO_no_init [/usr/lib/libc-2.22.so]
    72  ???:(below main) [/usr/lib/libc-2.22.so]
    69  ???:_IO_do_write@@GLIBC_2.1 [/usr/lib/libc-2.22.so]
    66  ???:_dl_protect_relro [/usr/lib/ld-2.22.so]
    64  ???:_IO_file_read [/usr/lib/libc-2.22.so]
    64  ???:__run_exit_handlers [/usr/lib/libc-2.22.so]
    62  ???:__init_cpu_features [/usr/lib/libc-2.22.so]
    61  assemble.c:shr [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:subn [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:and [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:or [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:xor [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:shl [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:sub [/home/gl/Projects/chip8/asm/asm]
    59  ???:_dl_debug_initialize [/usr/lib/ld-2.22.so]

--------------------------------------------------------------------------------
-- User-annotated source: tree.c
--------------------------------------------------------------------------------
Ir 

-- line 7 ----------------------------------------
 .  
 .  #include <stdio.h>
 .  #include <stdlib.h>
 .  #include <string.h>
 .  #include "tree.h"
 .  
 .  /* create new tnode */
 .  static tnode* new(const char* label, int address)
 4  {
12      tnode* n = malloc(sizeof(tnode));
825  => ???:_dl_runtime_resolve (1x)
416  => ???:malloc (2x)
14      n->label = strdup(label);
581  => ???:strdup (2x)
863  => ???:_dl_runtime_resolve (1x)
 4      n->address = address;
 2      n->less = NULL;
 2      n->more = NULL;
 .      return n;
 8  }
 .  
 .  /* add tnode to labels */
 .  static tnode* add(tnode* labels, tnode* n)
15  {
10      if(labels == NULL) return n;
10      int cmp = strcmp(n->label, labels->label);
839  => ???:_dl_runtime_resolve (1x)
 9  => ???:__GI_strcmp (1x)
 2      if(cmp == 0); /* do nothing */
 2      else if(cmp < 0) labels->less = add(labels->less, n);
 6      else labels->more = add(labels->more, n);
13  => tree.c:add.lto_priv.0'2 (1x)
 2      return labels;
12  }
 .  
 .  /* get tnode from labels */
 .  static tnode* get(tnode* labels, const char* label)
25  {
10      if(labels == NULL) return NULL;
30      int cmp = strcmp(label, labels->label);
195  => ???:__GI_strcmp (5x)
15      if(cmp == 0) return labels;
 .      else if(cmp < 0) return get(labels->less, label);
 .      else return get(labels->more, label);
20  }
 .  
 .  /* delete all tnodes in labels */
 .  static void delete(tnode* labels)
15  {
10      if(labels == NULL) return;
 6      delete(labels->less);
 8  => tree.c:delete'2 (1x)
 6      delete(labels->more);
240  => tree.c:delete'2 (1x)
 8      free(labels->label);
101  => ???:free (1x)
 8      free(labels);
101  => ???:free (1x)
15  }
 .  
 .  /* print all tnodes in labels */
 .  static void print(tnode* labels)
15  {
10      if(labels == NULL) return;
 6      print(labels->less);
 8  => tree.c:print'2 (1x)
16      printf("%04X: %s\n", labels->address, labels->label);
1,703  => ???:printf (1x)
863  => ???:_dl_runtime_resolve (1x)
 8      print(labels->more);
1,340  => tree.c:print'2 (1x)
15  }
 .  
 .  const struct tree tree = { new, add, get, delete, print };

--------------------------------------------------------------------------------
Ir 
--------------------------------------------------------------------------------
 0  percentage of events annotated

