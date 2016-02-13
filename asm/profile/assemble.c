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
User annotated:   assemble.c
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
    98  ???:_IO_cleanup [/usr/lib/libc-2.22.so]
    98  ???:_IO_file_open [/usr/lib/libc-2.22.so]
    97  ???:_IO_default_xsputn [/usr/lib/libc-2.22.so]
    96  assemble.c:jp [/home/gl/Projects/chip8/asm/asm]
    92  ???:_dl_init [/usr/lib/ld-2.22.so]
    90  ???:__fopen_internal [/usr/lib/libc-2.22.so]
    89  ???:init_tls [/usr/lib/ld-2.22.so]
    88  ???:_IO_switch_to_get_mode [/usr/lib/libc-2.22.so]
    87  ???:_dl_determine_tlsoffset [/usr/lib/ld-2.22.so]
    87  ???:_IO_flush_all_lockp [/usr/lib/libc-2.22.so]
    85  ???:sbrk [/usr/lib/libc-2.22.so]
    81  ???:_dl_discover_osversion [/usr/lib/ld-2.22.so]
    81  assemble.c:rnd [/home/gl/Projects/chip8/asm/asm]
    77  ???:init_cacheinfo [/usr/lib/libc-2.22.so]
    77  ???:_dl_check_all_versions [/usr/lib/ld-2.22.so]
    76  ???:_IO_no_init [/usr/lib/libc-2.22.so]
    76  ???:_IO_padn [/usr/lib/libc-2.22.so]
    72  ???:(below main) [/usr/lib/libc-2.22.so]
    69  ???:_IO_do_write@@GLIBC_2.1 [/usr/lib/libc-2.22.so]
    66  ???:_dl_protect_relro [/usr/lib/ld-2.22.so]
    64  ???:_IO_file_read [/usr/lib/libc-2.22.so]
    64  ???:__run_exit_handlers [/usr/lib/libc-2.22.so]
    62  ???:__init_cpu_features [/usr/lib/libc-2.22.so]
    61  assemble.c:shr [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:and [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:sub [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:subn [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:or [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:shl [/home/gl/Projects/chip8/asm/asm]
    61  assemble.c:xor [/home/gl/Projects/chip8/asm/asm]
    59  ???:_dl_debug_initialize [/usr/lib/ld-2.22.so]

--------------------------------------------------------------------------------
-- User-annotated source: assemble.c
--------------------------------------------------------------------------------
 Ir 

-- line 18 ----------------------------------------
  .         isxdigit(a[5]))
  .             fprintf(hexid, "%s", a+2);
  .      else return 1;
  .      return 0;
  .  }
  .  
  .  /* addition */
  .  static int add(char* o, tnode* labels, FILE* hexid)
 15  {
  .      (void)labels;
 15      char* a = strtok(o, "\t ,");
411  => ???:strtok (3x)
 18      char* b = strtok(NULL, "\t ,");
428  => ???:strtok (3x)
  .      /* ADD Vx, Vy */
 56      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
 14  => ???:__ctype_b_loc (2x)
 20         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  8             fprintf(hexid, "8%c%c4", a[1], b[1]);
584  => ???:fprintf (1x)
  .      /* ADD I, Vx */
  .      else
  8      if(strlen(a)==1 && a[0]=='I' &&
 15         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  7  => ???:__ctype_b_loc (1x)
  7             fprintf(hexid, "F%c1E", b[1]);
450  => ???:fprintf (1x)
  .      /* ADD Vx, byte */
  .      else
 13      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
  7  => ???:__ctype_b_loc (1x)
 22         strlen(b)==4 && strncmp(b,"0x",2)==0 &&
  3         isxdigit(b[2]) &&
  1         isxdigit(b[3]))
 10             fprintf(hexid, "7%c%s", a[1], b+2);
600  => ???:fprintf (1x)
  .      else return 1;
  4      return 0;
 21  }
  .  
  .  /* bitwise and */
  .  static int and(char* o, tnode* labels, FILE* hexid)
  3  {
  .      (void)labels;
  6      char* a = strtok(o, "\t ,");
137  => ???:strtok (1x)
  6      char* b = strtok(NULL, "\t ,");
141  => ???:strtok (1x)
  .      /* AND Vx, Vy */
 20      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
  7  => ???:__ctype_b_loc (1x)
 12         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  8             fprintf(hexid, "8%c%c2", a[1], b[1]);
577  => ???:fprintf (1x)
  .      else return 1;
  2      return 0;
  4  }
  .  
  .  /* call subroutine */
  .  static int call(char* o, tnode* labels, FILE* hexid)
  1  {
  4      char* a = strtok(o, "\t ,");
138  => ???:strtok (1x)
  5      tnode* found = tree.get(labels, a);
 59  => /home/gl/Projects/chip8/asm/tree.c:get (1x)
  .      /* CALL ad */
  2      if(found)
  7          fprintf(hexid, "2%03X", found->address);
634  => ???:fprintf (1x)
  .      else return 2;
  2      return 0;
  2  }
  .  
  .  /* clear display */
  .  static int cls(char* o, tnode* labels, FILE* hexid)
  1  {
  .      (void)o, (void)hexid, (void)labels;
  .      /* CLS */
 10      fprintf(hexid, "00E0");
140  => ???:fwrite (1x)
825  => ???:_dl_runtime_resolve (1x)
  .      return 0;
  3  }
  .  
  .  /* draw sprite */
  .  static int drw(char* o, tnode* labels, FILE* hexid)
  5  {
  .      (void)labels;
  5      char* a = strtok(o, "\t ,");
137  => ???:strtok (1x)
  6      char* b = strtok(NULL, "\t ,");
141  => ???:strtok (1x)
  6      char* c = strtok(NULL, "\t ,");
137  => ???:strtok (1x)
  .      /* DRW Vx, Vy, nib */
 20      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
  7  => ???:__ctype_b_loc (1x)
 15         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]) &&
 23         strlen(c)==3 && strncmp(c, "0x", 2)==0 && isxdigit(c[2]))
 12             fprintf(hexid, "D%c%c%c", a[1], b[1], c[2]);
679  => ???:fprintf (1x)
  .      else return 1;
  .      return 0;
  7  }
  .  
  .  /* jump */
  .  static int jp(char* o, tnode* labels, FILE* hexid)
  8  {
  .      tnode* found;
 12      char* a = strtok(o, "\t ,");
287  => ???:strtok (2x)
 12      char* b = strtok(NULL, "\t ,");
308  => ???:strtok (2x)
  .      /* JP V0, ad */
 31      if(strlen(a)==2 && a[0]=='V' && a[1]=='0' &&
 59  => /home/gl/Projects/chip8/asm/tree.c:get (1x)
  .         (found = tree.get(labels, b)))
  7             fprintf(hexid, "B%03X", found->address);
634  => ???:fprintf (1x)
  .      /* JP ad */
  .      else
  7      if((found = tree.get(labels, a)))
 59  => /home/gl/Projects/chip8/asm/tree.c:get (1x)
  7          fprintf(hexid, "1%03X", found->address);
639  => ???:fprintf (1x)
  .      else return 2;
  4      return 0;
  8  }
  .  
  .  /* load */
  .  static int ld(char* o, tnode* labels, FILE* hexid)
 55  {
  .      tnode* found;
 55      char* a = strtok(o, "\t ,");
1,542  => ???:strtok (11x)
 66      char* b = strtok(NULL, "\t ,");
1,557  => ???:strtok (11x)
  .      /* LD DT, Vx */
137      if(strlen(a)==2 && a[0]=='D' && a[1]=='T' &&
 16         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  7  => ???:__ctype_b_loc (1x)
  7             fprintf(hexid, "F%c15", b[1]);
444  => ???:fprintf (1x)
  .      /* LD ST, Vx */
  .      else
 36      if(strlen(a)==2 && a[0]=='S' && a[1]=='T' &&
 17         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  7  => ???:__ctype_b_loc (1x)
  7             fprintf(hexid, "F%c18", b[1]);
450  => ???:fprintf (1x)
  .      /* LD F, Vx */
  .      else
 26      if(strlen(a)==1 && a[0]=='F' &&
 17         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  7  => ???:__ctype_b_loc (1x)
  7             fprintf(hexid, "F%c29", b[1]);
444  => ???:fprintf (1x)
  .      /* LD B, Vx */
  .      else
 22      if(strlen(a)==1 && a[0]=='B' &&
 17         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  7  => ???:__ctype_b_loc (1x)
  7             fprintf(hexid, "F%c33", b[1]);
450  => ???:fprintf (1x)
  .      /* LD Vx, DT */
  .      else
 69      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
 35  => ???:__ctype_b_loc (5x)
 40         strlen(b)==2 && b[0]=='D' && b[1]=='T')
  8             fprintf(hexid, "F%c07", a[1]);
444  => ???:fprintf (1x)
  .      /* LD Vx, [I] */
  .      else
 56      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
 28  => ???:__ctype_b_loc (4x)
 29         strlen(b)==3 &&
  2         b[0]=='[' &&
  2         b[1]=='I' &&
  .         b[2]==']')
  8             fprintf(hexid, "F%c65", a[1]);
450  => ???:fprintf (1x)
  .      /* LD Vx, K */
  .      else
 43      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
 21  => ???:__ctype_b_loc (3x)
 22         strlen(b)==1 && b[0]=='K')
  8             fprintf(hexid, "F%c0A", a[1]);
444  => ???:fprintf (1x)
  .      /* LD Vx, Vy */
  .      else
 30      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
 14  => ???:__ctype_b_loc (2x)
 21         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  8             fprintf(hexid, "8%c%c0", a[1], b[1]);
577  => ???:fprintf (1x)
  .      /* LD Vx, byte */
  .      else
 18      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
  7  => ???:__ctype_b_loc (1x)
 24        (strlen(b)==4 && strncmp(b, "0x", 2))==0 &&
  4        isxdigit(b[2]) &&
  1        isxdigit(b[3]))
 11             fprintf(hexid, "6%c%s", a[1], b+2);
616  => ???:fprintf (1x)
  .      /* LD [I], Vx */
  .      else
  6      if(strlen(a)==3 &&
  2         a[0]=='[' &&
  2         a[1]=='I' &&
  2         a[2]==']' &&
 17         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  7  => ???:__ctype_b_loc (1x)
  7             fprintf(hexid, "F%c55", b[1]);
453  => ???:fprintf (1x)
  .      /* LD I, ad */
  .      else
  4      if(strlen(a)==1 && a[0]=='I')
  .      {
  7         if((found = tree.get(labels, b)))
 59  => /home/gl/Projects/chip8/asm/tree.c:get (1x)
  7             fprintf(hexid, "A%03X", found->address);
650  => ???:fprintf (1x)
  .         else return 2;
  .      }
  .      else return 1;
 20      return 0;
 77  }
  .  
  .  /* bitwise or */
  .  static int or(char* o, tnode* labels, FILE* hexid)
  3  {
  .      (void)labels;
  6      char* a = strtok(o, "\t ,");
141  => ???:strtok (1x)
  6      char* b = strtok(NULL, "\t ,");
141  => ???:strtok (1x)
  .      /* OR Vx, Vy */
 20      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
  7  => ???:__ctype_b_loc (1x)
 12         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  8             fprintf(hexid, "8%c%c1", a[1], b[1]);
585  => ???:fprintf (1x)
  .      else return 1;
  2      return 0;
  4  }
  .  
  .  /* return from subroutine */
  .  static int ret(char* o, tnode* labels, FILE* hexid)
  1  {
  .      (void)o, (void)hexid, (void)labels;
  .      /* RET */
  6      fprintf(hexid, "00EE");
140  => ???:fwrite (1x)
  .      return 0;
  3  }
  .  
  .  /* random number with mask */
  .  static int rnd(char* o, tnode* labels, FILE* hexid)
  5  {
  .      (void)labels;
  5      char* a = strtok(o, "\t ,");
137  => ???:strtok (1x)
  6      char* b = strtok(NULL, "\t ,");
146  => ???:strtok (1x)
  .      /* RND Vx, byte */
 21      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
  7  => ???:__ctype_b_loc (1x)
 22         strlen(b)==4 && strncmp(b, "0x", 2)==0 &&
  3         isxdigit(b[2]) &&
  1         isxdigit(b[3]))
 11             fprintf(hexid, "C%c%s", a[1], b+2);
601  => ???:fprintf (1x)
  .      else return 1;
  .      return 0;
  7  }
  .  
  .  /* skip instruction if... */
  .  static int se(char* o, tnode* labels, FILE* hexid)
 10  {
  .      (void)labels;
 10      char* a = strtok(o, "\t ,");
282  => ???:strtok (2x)
 12      char* b = strtok(NULL, "\t ,");
287  => ???:strtok (2x)
  .      /* SE Vx, Vy */
 50      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
 14  => ???:__ctype_b_loc (2x)
922  => ???:_dl_runtime_resolve (1x)
 20         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  8             fprintf(hexid, "5%c%c0", a[1], b[1]);
585  => ???:fprintf (1x)
  .      /* SE Vx, byte */
  .      else
 13      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
  7  => ???:__ctype_b_loc (1x)
 22         strlen(b)==4 && strncmp(b, "0x", 2)==0 &&
  3         isxdigit(b[2]) &&
  1         isxdigit(b[3]))
 10             fprintf(hexid, "3%c%s", a[1], b+2);
600  => ???:fprintf (1x)
  .      else return 1;
  2      return 0;
 14  }
  .  
  .  /* shift left */
  .  static int shl(char* o, tnode* labels, FILE* hexid)
  3  {
  .      (void)labels;
  6      char* a = strtok(o, "\t ,");
137  => ???:strtok (1x)
  6      char* b = strtok(NULL, "\t ,");
141  => ???:strtok (1x)
  .      /* SHL Vx, Vy */
 20      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
  7  => ???:__ctype_b_loc (1x)
 12         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  8             fprintf(hexid, "8%c%cE", a[1], b[1]);
577  => ???:fprintf (1x)
  .      else return 1;
  2      return 0;
  4  }
  .  
  .  /* shift right */
  .  static int shr(char* o, tnode* labels, FILE* hexid)
  3  {
  .      (void)labels;
  6      char* a = strtok(o, "\t ,");
137  => ???:strtok (1x)
  6      char* b = strtok(NULL, "\t ,");
141  => ???:strtok (1x)
  .      /* SHR Vx, Vy */
 20      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
  7  => ???:__ctype_b_loc (1x)
 12         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  8             fprintf(hexid, "8%c%c6", a[1], b[1]);
584  => ???:fprintf (1x)
  .      else return 1;
  2      return 0;
  4  }
  .  
  .  /* skip instruction if keypress */
  .  static int skp(char* o, tnode* labels, FILE* hexid)
  3  {
  .      (void)labels;
  5      char* a = strtok(o, "\t ,");
133  => ???:strtok (1x)
  .      /* SKP Vx */
 18      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]))
  7  => ???:__ctype_b_loc (1x)
  7          fprintf(hexid, "E%c9E", a[1]);
460  => ???:fprintf (1x)
  .      else return 1;
  2      return 0;
  4  }
  .  
  .  /* do not skip instruction if keypress */
  .  static int sknp(char* o, tnode* labels, FILE* hexid)
  3  {
  .      (void)labels;
  5      char* a = strtok(o, "\t ,");
137  => ???:strtok (1x)
  .      /* SKNP Vx */
 18      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]))
  7  => ???:__ctype_b_loc (1x)
  7          fprintf(hexid, "E%cA1", a[1]);
453  => ???:fprintf (1x)
  .      else return 1;
  2      return 0;
  4  }
  .  
  .  /* do not skip instruction if... */
  .  static int sne(char* o, tnode* labels, FILE* hexid)
 10  {
  .      (void)labels;
 10      char* a = strtok(o, "\t ,");
274  => ???:strtok (2x)
 12      char* b = strtok(NULL, "\t ,");
287  => ???:strtok (2x)
  .      /* SNE Vx, Vy */
 46      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
 14  => ???:__ctype_b_loc (2x)
 20         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  8             fprintf(hexid, "9%c%c0", a[1], b[1]);
595  => ???:fprintf (1x)
  .      /* SNE Vx, byte */
  .      else
 13      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
  7  => ???:__ctype_b_loc (1x)
 22         strlen(b)==4 && strncmp(b, "0x", 2)==0 &&
  3         isxdigit(b[2]) &&
  1         isxdigit(b[3]))
 10             fprintf(hexid, "4%c%s", a[1], b+2);
608  => ???:fprintf (1x)
  .      else return 1;
  2      return 0;
 14  }
  .  
  .  /* subtract */
  .  static int sub(char* o, tnode* labels, FILE* hexid)
  3  {
  .      (void)labels;
  6      char* a = strtok(o, "\t ,");
137  => ???:strtok (1x)
  6      char* b = strtok(NULL, "\t ,");
141  => ???:strtok (1x)
  .      /* SUB Vx, Vy */
 20      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
  7  => ???:__ctype_b_loc (1x)
 12         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  8             fprintf(hexid, "8%c%c5", a[1], b[1]);
584  => ???:fprintf (1x)
  .      else return 1;
  2      return 0;
  4  }
  .  
  .  /* reverse subtract */
  .  static int subn(char* o, tnode* labels, FILE* hexid)
  3  {
  .      (void)labels;
  6      char* a = strtok(o, "\t ,");
133  => ???:strtok (1x)
  6      char* b = strtok(NULL, "\t ,");
141  => ???:strtok (1x)
  .      /* SUBN Vx, Vy */
 20      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
  7  => ???:__ctype_b_loc (1x)
 12         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  8             fprintf(hexid, "8%c%c7", a[1], b[1]);
585  => ???:fprintf (1x)
  .      else return 1;
  2      return 0;
  4  }
  .  
  .  /* exslusive or */
  .  static int xor(char* o, tnode* labels, FILE* hexid)
  3  {
  .      (void)labels;
  6      char* a = strtok(o, "\t ,");
137  => ???:strtok (1x)
  6      char* b = strtok(NULL, "\t ,");
141  => ???:strtok (1x)
  .      /* XOR Vx, Vy */
 20      if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
  7  => ???:__ctype_b_loc (1x)
 12         strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
  8             fprintf(hexid, "8%c%c3", a[1], b[1]);
577  => ???:fprintf (1x)
  .      else return 1;
  2      return 0;
  4  }
  .  
  .  /* mnemonic functions */
  .  static int (*function[])(char* o, tnode* labels, FILE* hexid) =
  .  {
  .      add, and, call, cls, drw, jp, ld, or, ret, rnd, se, shl,
  .      shr, skp, sknp, sne, sub, subn, _uint, xor
  .  };
  .  
  .  static int execute(int (*fun)(char*, tnode*, FILE*), char* o, tnode* labels, FILE* hexid)
 34  {
136      return fun(o, labels, hexid);
1,200  => assemble.c:drw (1x)
930  => assemble.c:sub (1x)
2,082  => assemble.c:jp (2x)
927  => assemble.c:subn (1x)
854  => assemble.c:call (1x)
923  => assemble.c:xor (1x)
150  => assemble.c:ret (1x)
923  => assemble.c:and (1x)
930  => assemble.c:shr (1x)
972  => assemble.c:rnd (1x)
979  => assemble.c:cls (1x)
923  => assemble.c:shl (1x)
2,872  => assemble.c:se (2x)
9,767  => assemble.c:ld (11x)
636  => assemble.c:sknp (1x)
935  => assemble.c:or (1x)
639  => assemble.c:skp (1x)
1,956  => assemble.c:sne (2x)
2,737  => assemble.c:add.lto_priv.1 (3x)
 68  }
  .  
  .  /* for use with bsearch */
  .  static int compare(const void* a, const void* b)
136  {
544      return strcmp((const char*)a, *(const char**)b);
2,111  => ???:__GI_strcmp (136x)
272  }
  .  
  .  /* supported chip8 mnemonics */
  .  const char* const mnemonic[] =
  .  { /*   0     1      2     3     4    5    6    7     8     9   10    11    12     13    14    15    16     17     18    19 */
  .      "ADD","AND","CALL","CLS","DRW","JP","LD","OR","RET","RND","SE","SHL","SHR","SKNP","SKP","SNE","SUB","SUBN","UINT","XOR"
  .  };
  .  
  .  /* assembles given a mnemonic m, an operand o, a label tree, and an output file; returns error code */
  .  int assemble(char* m, char* o, tnode* labels, FILE* hexid)
272  {
  .      int error = 0;
  .      /* check if 'm' is a supported mnemonic */
  .      const char* const* supported = bsearch(m, mnemonic, sizeof(mnemonic)/sizeof(char*), sizeof(char*), compare);
  .      /* if 'm' is not supported return an error */
 68      if(!supported) return error = 3;
  .      /* if the operand is missing and the operand is not CLS or RET then return "a missing operand" error */
 68      int index = supported - mnemonic;
 74      if(o == NULL && (index != 3 && index != 8)) return error = 4;
  .      /* execute */
238      error = execute(function[index], o, labels, hexid);
31,573  => assemble.c:execute (34x)
  .      /* ... and report any other errors */
310      if(!error) fputc('\n', hexid);
1,836  => ???:fputc (34x)
826  => ???:_dl_runtime_resolve (1x)
 68      return error;
204  }

--------------------------------------------------------------------------------
Ir 
--------------------------------------------------------------------------------
 2  percentage of events annotated

