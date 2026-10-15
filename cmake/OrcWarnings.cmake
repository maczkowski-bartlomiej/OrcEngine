# Applies the project's warning set to a target (GCC / Clang / MSVC).
# Roughly mirrors the old premake setup: everything on, unused parameters ignored.
function(orc_enable_warnings target)
    target_compile_options(${target} PRIVATE
        "$<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wno-unused-parameter>"
        "$<$<CXX_COMPILER_ID:MSVC>:/W4;/wd4100>"
    )
endfunction()
