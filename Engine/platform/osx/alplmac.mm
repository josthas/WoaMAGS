#import <Cocoa/Cocoa.h>

#include <stdio.h>
#include <string>

void AGSMacInitPaths(char gamename[256], char appdata[PATH_MAX])
{
  NSAutoreleasePool *p = [[NSAutoreleasePool alloc] init];

  NSBundle *bundle = [NSBundle mainBundle];
  NSString *resourcedir = [bundle resourcePath];
  [[NSFileManager defaultManager] changeCurrentDirectoryPath:resourcedir];

  strcpy(gamename, "agsgame.dat");

  NSURL *path = [[NSFileManager defaultManager] URLForDirectory:NSApplicationSupportDirectory inDomain:NSUserDomainMask appropriateForURL:nil create:YES error:NULL];
  snprintf(appdata, PATH_MAX, "%s", [[path path] UTF8String]);
#ifdef MAC_STORE
  int n = 0;
  int index = -1;
  bool bCont = true;
  int slashcount = 0;
  for(int n = 0; bCont == true; n++)
  {
    if(appdata[n] == '/')
    {
      slashcount++;
      if(slashcount == 4)
        index = n+1;
    }
    if(appdata[n] == '\0')
      bCont = false;
  }
  
  if(index != -1)
    appdata[index] = '\0';
  
  char* ptmp = appdata;
  snprintf(appdata, PATH_MAX, "%sContainers/com.mpdigital.kathyrain/Data/Library/Application Support", appdata);
#endif 
  //std::string tmp = appdata;
  
  ////std::string tmp = tmp.copy(appdata, PATH_MAX);
  ////const char* pTest = tmp.c_str();
  
  //strcpy(appdata, pTest);
  //tmp = tmp.replace(tmp.find_first_of("Application Support"), tmp.length(), "");
  
  [p drain];
}

void AGSMacGetBundleDir(char gamepath[PATH_MAX])
{
  NSAutoreleasePool *p = [[NSAutoreleasePool alloc] init];

  NSBundle *bundle = [NSBundle mainBundle];
  NSString *bundleDir = [bundle bundlePath];

  NSString *parentDir = [bundleDir stringByDeletingLastPathComponent];
  strcpy(gamepath, [parentDir UTF8String]);

  [p drain];
}