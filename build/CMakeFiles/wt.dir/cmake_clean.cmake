FILE(REMOVE_RECURSE
  "CMakeFiles/wt"
  "CMakeFiles/wt-complete"
  "../externalLibraries/src/wt-stamp/wt-install"
  "../externalLibraries/src/wt-stamp/wt-mkdir"
  "../externalLibraries/src/wt-stamp/wt-download"
  "../externalLibraries/src/wt-stamp/wt-update"
  "../externalLibraries/src/wt-stamp/wt-patch"
  "../externalLibraries/src/wt-stamp/wt-configure"
  "../externalLibraries/src/wt-stamp/wt-build"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/wt.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
