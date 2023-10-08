/* wcstok example */
#include <wchar.h>

int main ()
{
  wchar_t wcs[] = L"Mdr Wr Wr Wr Wr Wr Wd Mdl Ad Ad Mdr Wr Wr Wr Wd Mdl";
  wchar_t * pwc;
  wchar_t * pt;
  int compte = 0;
  wprintf (L"Splitting wide string \"%ls\" into tokens:\n",wcs);
  pwc = wcstok (wcs, L" ,.-");
  printf("cmpt : %d\n", compte++);
  while (pwc != NULL)
  {
    wprintf (L"%ls\n",pwc);
    pwc = wcstok (NULL, L" ,.-");
    printf("cmpt : %d\n", compte++);
  }
  return 0;
}