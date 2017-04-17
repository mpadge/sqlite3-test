#pragma once

#include <string>
#include <vector>
#include <map>
#include <sqlite3.h>

#include <curl/curl.h>

// [[Rcpp::depends(BH)]]
#include <Rcpp.h>

#include <boost/algorithm/string/replace.hpp>


//' compare_version_numbers
//'
//' Function to compare version numbers
//' First argument is compared to the second argument
//' Return value:
//' -1 = Argument one version lower than Argument two version
//' 0 = Argument one version equal to Argument two version
//' 1 = Argument one version higher than Argument two version
//'
//' @noRd
int compare_version_numbers (std::string vstro, std::string compvstro) {
  
  int versiondiff = 0;
  
  char *vstr = (char *)vstro.c_str();
  char *compvstr = (char *)compvstro.c_str();
  
  char *vstrtok, *compvstrtok;
  char *vstrtokptr, *compvstrtokptr;
  
  vstrtok = strtok_r(vstr, ".", &vstrtokptr);
  compvstrtok = strtok_r (compvstr, ".", &compvstrtokptr);

  if (atoi(vstrtok) < atoi(compvstrtok)) {
    versiondiff = -1;
  }
  else if (atoi(vstrtok) > atoi(compvstrtok)) {
    versiondiff = 1;
  }
  else {
    while (vstrtok != NULL && compvstrtok != NULL && versiondiff == 0) {

      vstrtok = strtok_r (NULL, ".", &vstrtokptr);
      compvstrtok = strtok_r (NULL, ".", &compvstrtokptr);

      if (vstrtok == NULL && compvstrtok == NULL) {
        versiondiff = 0;
      }
      else if (vstrtok == NULL && compvstrtok != NULL) {
        if (atoi(compvstrtok) == 0) {
          versiondiff = 0;
        }
        else {
          versiondiff = -1;
        }
      }
      else if (vstrtok != NULL && compvstrtok == NULL) {
        if (atoi(vstrtok) == 0) {
          versiondiff = 0;
        }
        else {
          versiondiff = 1;
        }
      }
      else if (atoi(vstrtok) < atoi(compvstrtok)) {
        versiondiff = -1;
      }
      else if (atoi(vstrtok) > atoi(compvstrtok)) {
        versiondiff = 1;
      }

    }

  }
  
  return versiondiff;

}

// write result of curl call to std::string
// http://stackoverflow.com/questions/2329571/c-libcurl-get-output-into-a-string
size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, 
        size_t nmemb, std::string *s)
{
    size_t newLength = size*nmemb;
    size_t oldLength = s->size();
    try
    {
        s->resize(oldLength + newLength);
    }
    catch(std::bad_alloc &e)
    {
        //handle memory problem
        return 0;
    }

    std::copy((char*)contents,(char*)contents+newLength,s->begin()+oldLength);
    return size*nmemb;
}

