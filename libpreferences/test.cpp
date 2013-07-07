#include "libpreferences.cpp"
#include <stdio.h>
int main()
{
	preferences p("test");
	p.writePreferenceByKey("hello", (char *)"world");
	p.writePreferenceByKey("hello2", (char *)"world3");
	p.writePreferenceByKey("hello", (char *)"world2");
	puts(p.getPreferenceByKey("hello"));
	p.close();
}
