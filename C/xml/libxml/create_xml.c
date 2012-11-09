#include <libxml/parser.h>
#include <libxml/tree.h>

#define CREATE_XML_FILE "./create_xml.xml"

int main(int argc, char* argv[])
{
	xmlDocPtr pdoc = NULL;
	xmlNodePtr proot_node = NULL;
	xmlNodePtr pnode = NULL;

	pdoc = xmlNewDoc(BAD_CAST "1.0");

	proot_node = xmlNewNode(NULL, BAD_CAST"vm");
	xmlNewProp(proot_node, NULL, NULL);

	xmlDocSetRootElement(pdoc, proot_node);

	pnode = xmlNewNode(NULL, BAD_CAST"resp");
	xmlNewProp(pnode, BAD_CAST"value", BAD_CAST"0");

	xmlAddChild(proot_node, pnode);

	xmlSaveFormatFileEnc(argc > 1 ? argv[1] : "-", pdoc, "UTF-8", 1);
	xmlSaveFile(CREATE_XML_FILE, pdoc);

	xmlFreeDoc(pdoc);
	xmlCleanupParser();

	xmlMemoryDump();
	return 0;
}
