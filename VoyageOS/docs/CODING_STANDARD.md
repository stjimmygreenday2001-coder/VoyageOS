# VoyageOS Coding Standards

## Naming Conventions

### C Code

**Variables:**
- Use `snake_case` for all variables
- Prefix static/internal variables with `_`
- Prefix kernel-internal functions with `k_`
- Prefix global variables with `g_`

```c
int page_count;
static int _internal_state;
void k_alloc(size_t size);
int g_total_memory;
```

**Functions:**
- Use `snake_case` for all functions
- Kernel functions: `k_function_name()`
- IDE functions: `ide_function_name()`
- Driver functions: `driver_function_name()`

```c
void *k_alloc(size_t size);
void ide_draw_screen(void);
void driver_keyboard_init(void);
```

**Types & Structs:**
- Use `PascalCase` for custom types
- Typedef struct names without `_t` suffix (use `struct_t` instead)
- Use meaningful descriptive names

```c
typedef struct {
    uint64_t base;
    uint32_t limit;
} gdt_entry_t;

struct page_table {
    uint64_t entries[512];
};
```

**Constants/Macros:**
- Use `UPPER_SNAKE_CASE` for macros and constants
- Prefix related constants with their purpose

```c
#define PAGE_SIZE 4096
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define SYSCALL_READ 0
#define SYSCALL_WRITE 1
```

### Assembly Code

**Labels:**
- Use `lower_snake_case` for labels
- Prefix section-related labels with section abbreviation

```asm
pm_enter:          ; Protected mode entry
lm_setup:          ; Long mode setup
isr_timer:         ; Interrupt service routine - timer
.loop:             ; Local label
```

**Comments:**
- Single-line: `;`
- Section markers: `;====` style

```asm
; Load GDT for protected mode transition
lgdt [gdt_pointer]
```

### VoyageC Code

**Variables:**
- Similar to C conventions but simpler
- No underscore prefixes needed in OS code

```voyagec
num counter = 0;
text message = "Hello";
ptr buffer = alloc(256);
```

**Functions:**
- Use `snake_case`
- Self-documenting names

```voyagec
fn print_welcome_message() { ... }
fn calculate_fibonacci(num n) -> num { ... }
fn handle_keyboard_interrupt(num key) { ... }
```

## Code Style

### C Code Formatting

**Indentation:**
- Use 4 spaces (no tabs)
- Consistent throughout the file

```c
if (condition) {
    statement;
    if (nested) {
        nested_statement;
    }
}
```

**Line Length:**
- Maximum 100 characters per line
- Break long lines appropriately

```c
// Good: readable
int result = function_call(arg1, arg2,
                           arg3, arg4);

// Bad: too long
int result = function_call(arg1, arg2, arg3, arg4, arg5, arg6);
```

**Braces:**
- Opening brace on same line (K&R style)
- Closing brace on new line

```c
void function_name(void) {
    statement;
    if (condition) {
        statement;
    }
}
```

**Comments:**
- Use `/* */` for multi-line
- Use `//` for single-line
- Document "why", not "what"

```c
// Good: explains reasoning
// Use a larger buffer to reduce syscall overhead
char buffer[8192];

// Bad: obvious from code
// Loop through array
for (int i = 0; i < len; i++) { ... }
```

**Function Style:**
- Return type on separate line (optional)
- Parameters properly formatted

```c
// Simple function
void simple_function(void) { ... }

// Multi-parameter function
void longer_function(
    int param1,
    void *pointer,
    size_t size
) {
    ...
}
```

### Assembly Formatting

**Indentation:**
- Labels at column 0
- Instructions indented 4 spaces
- Comments indented to column 40+

```asm
syscall_handler:           ; Label at column 0
    push rbp               ; Instruction indented
    mov rbp, rsp
    mov rax, [rdi]         ; Load argument
    pop rbp
    ret

    ; Comment for clarity
```

**Organization:**
- Group related code together
- Use section comments

```asm
; ====== IDT Setup ======
lidt [idt_pointer]
cli

; ====== GDT Setup ======
setup_gdt:
    ...
```

## Documentation Requirements

### Kernel Code

Every public function should have a documentation comment:

```c
/**
 * Allocate memory from the kernel heap.
 * @param size Number of bytes to allocate
 * @return Pointer to allocated memory, or NULL on failure
 */
void *k_alloc(size_t size);
```

### File Headers

Every source file should start with:

```c
/*
 * Filename.c - Brief description
 *
 * Longer description explaining what this file does,
 * what components it interacts with, and any important
 * implementation details.
 */
```

### Complex Algorithms

Document non-obvious implementation:

```c
// Page table walk: traverse 4-level page table structure
// to find physical address corresponding to virtual address
uint64_t virt_to_phys(uint64_t vaddr) {
    // Extract indices from virtual address (47:39, 38:30, 29:21, 20:12)
    int pml4_idx = (vaddr >> 39) & 0x1FF;
    int pdpt_idx = (vaddr >> 30) & 0x1FF;
    int pdt_idx = (vaddr >> 21) & 0x1FF;
    int pt_idx = (vaddr >> 12) & 0x1FF;
    
    // ... walk the tables ...
}
```

## Error Handling

### Return Codes

- Return negative values for errors (Unix convention)
- Return 0 for success
- Return positive values for specific codes

```c
#define SUCCESS 0
#define ERR_NO_MEMORY -1
#define ERR_NOT_FOUND -2
#define ERR_INVALID_PARAM -3

int operation(void) {
    if (!resources_available) {
        return ERR_NO_MEMORY;
    }
    return SUCCESS;
}
```

### Panic vs. Continue

- `k_panic()` for unrecoverable errors (double fault, invalid CPU state)
- Return error codes for recoverable errors

```c
// Unrecoverable: use panic
if (cpu_exception && no_handler) {
    k_panic("Unhandled CPU exception");
}

// Recoverable: use return code
int result = read_file(fd, buffer);
if (result < 0) {
    log("File read failed, retrying...");
    return result;
}
```

## Testing & Validation

### Before Committing

1. Code compiles without warnings
2. Tested booting VoyageOS
3. Boots to kernel main or IDE prompt
4. No memory leaks (check with appropriate tools)
5. Documentation is accurate

### Code Review Checklist

- [ ] Follows naming conventions
- [ ] Comments explain "why", not "what"
- [ ] No magic numbers (use constants)
- [ ] Error handling is appropriate
- [ ] Function documentation is present
- [ ] No compiler warnings

## Anti-patterns to Avoid

### ❌ Don't do these things:

1. **Magic numbers without explanation**
   ```c
   // Bad
   offset = addr + 0x1000;
   
   // Good
   offset = addr + PAGE_SIZE;
   ```

2. **Overly cryptic variable names**
   ```c
   // Bad
   int tmp, i, c, x;
   
   // Good
   int temp_result, iterator, character_count, x_coord;
   ```

3. **Massive functions**
   ```c
   // Bad: 500+ line function
   void process_everything(void) { ... }
   
   // Good: break into smaller pieces
   void setup_tables(void);
   void init_interrupts(void);
   void start_scheduler(void);
   ```

4. **Inconsistent style**
   ```c
   // Don't mix styles
   if(condition){statement;}
   if (condition) { statement; }
   if ( other ) { statement; }
   ```

5. **Assuming sizes of types**
   ```c
   // Bad: depends on compiler/platform
   int *ptr = (int *)malloc(4);
   
   // Good: explicit about size
   int *ptr = (int *)malloc(sizeof(int));
   uint32_t *ptr32 = (uint32_t *)malloc(sizeof(uint32_t));
   ```

---

*VoyageOS code should be readable, maintainable, and consistent.*
