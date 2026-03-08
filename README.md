# hope-json

Header-only typed JSON mapping library built in the Hope ecosystem.
It maps JSON to C++ structs (and back) using `hope-core` reflection and named field wrappers.
The project naming/style follows the conventions used in [glensand/hope](https://github.com/glensand/hope).

## Library purpose

`hope-json` is focused on:

- Strict and nullable field policies for predictable schema behavior.
- Compile-time typed wrappers around JSON primitives and arrays.
- Conversion between JSON text and aggregate C++ structs via `load` / `store`.
- Minimal setup with no runtime schema definitions.

## Requirements

- C++17 compiler
- CMake 3.11+ (project root) and 3.15+ (`lib/`)
- `hope-core` submodule (`ext/hope-core`)

## Build

```bash
git clone --recurse-submodules <repo-url>
cd hope-json
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build -V
```

## Quick start

```cpp
#include "hope/document.h"
#include "hope/types_macro.h"

struct profile final {
    SJSTRING(name);
    SJINT(age);
    NJREAL(score);
};

int main() {
    constexpr auto* json = R"({"name":"Gleb","age":30,"score":99.5})";

    auto object = hope::load<profile>(json);
    object.age = 31;

    const auto out = hope::store(object);
    return out.empty();
}
```

## Field model

The library uses named wrappers for each struct field:

- Strict fields: `SJINT`, `SJREAL`, `SJBOOL`, `SJSTRING`, `SJARRAY`, `SJOBJECT`
- Nullable fields: `NJINT`, `NJREAL`, `NJBOOL`, `NJSTRING`, `NJARRAY`, `NJOBJECT`

Strict fields require valid JSON values.
Nullable fields are optional and only serialized when present.

## Project layout

- `lib/hope`: public include path (`#include "hope/..."`)
- `test/`: unit tests (GoogleTest)
- `samples/document`: usage example
- `ext/hope-core`: Hope reflection/type utilities dependency

## Notes

- The project follows `hope` naming/style (`hope::...` namespace and `hope/...` headers).

## License

Distributed under the [MIT License](LICENSE).
