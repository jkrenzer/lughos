FILE(REMOVE_RECURSE
  "CMakeFiles/libharu"
  "CMakeFiles/libharu-complete"
  "../externalLibraries/src/libharu-stamp/libharu-install"
  "../externalLibraries/src/libharu-stamp/libharu-mkdir"
  "../externalLibraries/src/libharu-stamp/libharu-download"
  "../externalLibraries/src/libharu-stamp/libharu-update"
  "../externalLibraries/src/libharu-stamp/libharu-patch"
  "../externalLibraries/src/libharu-stamp/libharu-configure"
  "../externalLibraries/src/libharu-stamp/libharu-build"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/libharu.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
