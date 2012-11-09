import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.text.AttributedCharacterIterator.Attribute;
import java.util.HashMap;
import java.util.Map;

import javax.management.modelmbean.XMLParseException;

import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;
import org.xml.sax.helpers.XMLReaderAdapter;
import org.xml.sax.helpers.XMLReaderFactory;

public class MapTableHandler extends DefaultHandler {

	private static final String ROOT_ELEMENT = "rearview_mirror";
	private static final String RECORD = "record";
	private static final String PORT_TABLE = "port_table";

	private boolean isInRearView = false;
	private boolean isInPortTable = false;

	private Map<String, String> mapTable;

	public Map<String, String> getTable() {
		return mapTable;
	}

	@Override 
		public void startDocument() throws SAXException {
			super.startDocument();
			mapTable = new HashMap<String, String>();
		}

	@Override
		public void startElement(String uri, String localName, String qName,
				Attributes attributes) throws SAXException {
			super.startElement( uri, localName, qName, attributes);
			if(localName.equalsIgnoreCase(ROOT_ELEMENT)) {
				isInRearView = true;
				return;
			}

			if (localName.equalsIgnoreCase("array") && isInRearView == true) {
				if (attributes.getValue("name").equalsIgnoreCase(PORT_TABLE)) {
					isInPortTable = true;
					return;
				}
			}

			if (localName.equalsIgnoreCase(RECORD) && isInRearView == true && isInPortTable == true) {
				String cmd = attributes.getValue("cmd");
				String port = attributes.getValue("port");

				mapTable.put(cmd, port);
				return;
			}
		}

	@Override
		public void endElement(String uri, String localName, String qName) 
		throws SAXException {
		if (localName.equalsIgnoreCase(PORT_TABLE)) {
			isInPortTable = false;
			return;
		}

		if (localName.equalsIgnoreCase(ROOT_ELEMENT)) {
			isInRearView = false;
			return;
		}
		}


	public void parserXMLFile(String fileName) throws SAXException, IOException {
		XMLReader reader = XMLReaderFactory.createXMLReader();
		reader.setContentHandler(this);
		InputSource source = new InputSource(new FileInputStream(new File(fileName)));
		reader.parse(source);
	}

	/**
	 *      * @param args
	 *           * @throws IOException 
	 *                * @throws SAXException 
	 *                     */
	public static void main(String[] args) throws SAXException, IOException {
		MapTableHandler parser = new MapTableHandler();
		parser.parserXMLFile("rearview_mirror.xml");

		Map<String, String> map = parser.getTable();
		for (Object o : map.keySet()) {
			System.out.println(o+" : "+map.get(o));
		}
	}

}

