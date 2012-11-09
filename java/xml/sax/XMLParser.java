//package com.chris.sax.action;


import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.List;


import org.xml.sax.Attributes;
import org.xml.sax.ContentHandler;
import org.xml.sax.ErrorHandler;
import org.xml.sax.InputSource;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.XMLReaderFactory;

public class XMLParser
{
	//handler normal info.
	protected  PrintStream output =
		new PrintStream( new BufferedOutputStream( new FileOutputStream( java.io.FileDescriptor.out ), 128 ), true );
	//handler error info.
	protected  PrintStream error =
		new PrintStream( new BufferedOutputStream( new FileOutputStream( java.io.FileDescriptor.err ), 128 ), true );

	public void parserXMLFile(String fileName ) throws SAXException, IOException
	{
		XMLReader reader = XMLReaderFactory.createXMLReader();
		reader.setContentHandler( new MyContentHandler() );
		reader.setErrorHandler( new MyErrorHandler() );
		InputSource source = new InputSource( new FileInputStream( new File( fileName ) ) );
		reader.parse(source);
	}

	class MyErrorHandler implements ErrorHandler
	{

		public void error( SAXParseException exception ) throws SAXException
		{

			error.println( exception.getMessage() );
		}

		public void fatalError( SAXParseException exception ) throws SAXException
		{

			error.println( exception.getMessage() );
		}

		public void warning( SAXParseException exception ) throws SAXException
		{
			output.println( exception.getMessage() );

		}

	}

	class MyContentHandler implements ContentHandler
	{

		private StringBuffer  buffer = new StringBuffer();
		private String key;


		public void characters( char[] ch, int start, int length )
			throws SAXException
		{
			buffer.append( ch, start, length);

		}

		public void endDocument() throws SAXException
		{

		}

		public void endElement( String uri, String localName, String name )
			throws SAXException
		{

			if( key.equals( localName))
			{
				//output.print(buffer);
			}

			//output.print("</"+localName+">");
		}

		public void endPrefixMapping( String prefix ) throws SAXException
		{


		}

		public void ignorableWhitespace( char[] ch, int start, int length )
			throws SAXException
		{

		}

		public void processingInstruction( String target, String data )
			throws SAXException
		{

		}

		public void setDocumentLocator( Locator locator )
		{

		}

		public void skippedEntity( String name ) throws SAXException
		{

		}

		public void startDocument() throws SAXException
		{
			output.println("<xml version=\"1.0\" encoding=\"utf-8\"?>");
		}

		public void startElement( String uri, String localName, String name,
				Attributes atts ) throws SAXException
		{
			//uri is identifier of namespace
			//name-->prefix:localName

			buffer.delete(0, buffer.length());
			key = localName;

		//	output.print("<" +localName );			
			output.println("start_element: " + name);			
			for( int i = 0; i < atts.getLength(); i++ )
			{
				String attrName = atts.getLocalName( i );
				String attrValue = atts.getValue( i );
				//output.print(" " + attrName + "=" + attrValue );
			}
			//output.print(">");

		}

		public void startPrefixMapping( String prefix, String uri )
			throws SAXException
		{

		}		
	}

	public static void main( String[] args ) throws Exception, IOException
	{

		XMLParser parser = new XMLParser();
		parser.parserXMLFile("books.xml");
	}
}

