# VoyageOS Project Roadmap

## Vision
A self-hosted x86-64 operating system with an integrated development environment, emphasizing simplicity, self-modification, and creative freedom.

## Release Timeline

### v0.1 - Foundation (Current)
**Status:** In Development

**Bootloader:**
- [x] Real mode initialization
- [x] A20 gate enabling
- [x] Protected mode entry
- [x] Long mode (64-bit) setup
- [ ] Memory detection
- [ ] Multiple boot option support

**Kernel:**
- [x] Basic kernel entry
- [x] VGA text output
- [x] Exception handling (stub)
- [ ] Interrupt handling (full IDT)
- [ ] Memory management (paging)
- [ ] Process management
- [ ] Syscall interface
- [ ] Interrupt masking/handling

**VoyageIDE:**
- [x] Screen layout and boxes
- [ ] Text editor (functional)
- [ ] File operations (open/save)
- [ ] Syntax highlighting
- [ ] Code compilation triggering
- [ ] Error display

**Compiler (VoyageC):**
- [x] Lexer structure
- [ ] Parser (token → AST)
- [ ] Type checker
- [ ] Code generator (AST → x86-64)
- [ ] Assembler integration
- [ ] Linker integration

**Tools:**
- [ ] Command shell
- [ ] File manager
- [ ] Text viewer
- [ ] System monitor

---

### v0.2 - Functional Kernel (Q2 2026)

**Bootloader Enhancements:**
- [ ] Detect system memory (INT 0x15)
- [ ] Load kernel from disk (ATA/SATA)
- [ ] Multicore SMP support
- [ ] ACPI table parsing

**Kernel Features:**
- [ ] Complete IDT/ISR handling
- [ ] Paging and virtual memory
- [ ] Heap allocator (kmalloc/kfree)
- [ ] Process/task scheduler
- [ ] Context switching
- [ ] Syscall interface (read, write, open, close, fork, exec)
- [ ] Basic file operations

**VoyageIDE v2:**
- [ ] Multi-line editing
- [ ] Syntax highlighting (VoyageC)
- [ ] File browser
- [ ] Basic REPL
- [ ] Compile and run
- [ ] Debugger (breakpoints, stepping)

**VoyageC Compiler v2:**
- [ ] Full parser (recursive descent)
- [ ] Type checking
- [ ] Code generation for common patterns
- [ ] Optimization passes
- [ ] Standard library (stdio, stdlib, string)

---

### v0.3 - Filesystem & I/O (Q3 2026)

**Filesystem:**
- [ ] VoyageFS (custom filesystem)
- [ ] Disk I/O drivers
- [ ] File system cache
- [ ] Directory support
- [ ] File permissions (basic)

**I/O Drivers:**
- [ ] Keyboard driver (full)
- [ ] Mouse driver
- [ ] ATA/SATA hard drive
- [ ] USB support (future)

**Kernel Features:**
- [ ] Signals (SIGTERM, SIGKILL, etc.)
- [ ] Process groups
- [ ] Standard input/output/error
- [ ] Environment variables

---

### v0.4 - Advanced Features (Q4 2026)

**Networking:**
- [ ] Network stack (TCP/IP)
- [ ] Network drivers (Ethernet)
- [ ] Socket API
- [ ] Network utilities (ping, netcat, etc.)

**Security:**
- [ ] User/group system
- [ ] Permission system
- [ ] Memory protection enhancements
- [ ] Stack canaries

**IDE Enhancements:**
- [ ] Plugin system
- [ ] Multi-window support
- [ ] Project management
- [ ] Code completion
- [ ] Version control integration

---

### v1.0 - Stable Release (2027)

**Stability:**
- [ ] Bug fixes from 0.4 and earlier
- [ ] Performance optimization
- [ ] Documentation complete
- [ ] SDK release

**Applications:**
- [ ] Text editors (vi-like)
- [ ] Programming language support (expanded)
- [ ] System utilities
- [ ] Example programs

**Community:**
- [ ] Version control repository
- [ ] Issue tracker
- [ ] Wiki/documentation site
- [ ] Forum/community

---

## Future Considerations (Post v1.0)

### Longer-term Goals

**Language Evolution:**
- Multi-language support
- JIT compilation support
- Scripting API

**Advanced OS Features:**
- Virtual machine support
- Container-like isolation
- Distributed execution

**Graphics & UI:**
- Graphical window manager
- Modern graphics subsystem
- 3D rendering support

**Tools & Infrastructure:**
- Package manager
- Build system enhancements
- Testing framework
- Profiler

**Performance:**
- SMP/multicore optimization
- Cache efficiency improvements
- Vectorization support

---

## Development Priorities

### High Priority
1. Functional bootloader → kernel transition ✓
2. Memory management (paging, allocation)
3. Interrupt/exception handling
4. IDE usability
5. Compiler functionality (parse → codegen)

### Medium Priority
1. Filesystem support
2. Syscall interface
3. Process/task system
4. Standard library

### Lower Priority
1. Networking
2. Security hardening
3. Advanced IDE features
4. Graphics/UI

---

## Known Issues & Limitations

### Current Limitations (v0.1)

- **No real memory management** - needs paging implementation
- **No real interrupt handling** - basic exception stubs only
- **IDE is non-interactive** - display-only in current state
- **Compiler incomplete** - lexer structure only
- **Single-task only** - no process management
- **No filesystem** - would need disk I/O

### Technical Debt

- Bootloader could be optimized
- Kernel entry needs proper stack alignment
- Need GDT, IDT, TSS in C structures
- Error handling minimal
- Documentation needs examples

---

## Contribution Areas

### For New Contributors

**Easy:**
- Documentation improvements
- Example programs (VoyageC)
- Building IDE UI components
- Cross-platform build script improvements

**Medium:**
- Memory allocator implementation
- Interrupt handler implementation
- File I/O syscalls
- Shell command implementation

**Hard:**
- Full compiler pipeline
- SMP support
- Filesystem design and implementation
- Performance optimization

---

*VoyageOS is a long-term hobby project with ambitious goals. Progress depends on available time and community contributions.*
