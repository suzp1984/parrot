/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * History:
 * ================================================================
 * 2011-01-14 suzhenxing <suzhenxing@topwise3g.com> created
 *
 */
package com.topwise.aftersale;

import java.io.File;
import java.util.List;
import java.util.Iterator;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

public class SaxXmlParser {

    final File mFile;

    public SaxXmlParser(String fileName) {
        this.mFile = new File(fileName);
    }

    public SaxXmlParser(File file) {
    	this.mFile = file;
    }
    public List<AfterSaleServer> parse() {
        SAXParserFactory factory = SAXParserFactory.newInstance();
        try {
            SAXParser parser = factory.newSAXParser();
            SimpleHandler handler = new SimpleHandler();
            parser.parse(getInputStream(), handler);
            return handler.getServers();
            
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    private File getInputStream() {
        return mFile;
    }

    private static void Usage() {
        System.out.println("Usage(): bin_file xml_file\n");
    }

    public static void main(String[] args) {
        //System.out.println("in main: args's size is " + args.length);
        
        if (args.length < 1) {
            Usage();
            return;
        }

        //System.out.println("**** " + args[0]);
        SaxXmlParser parser = new SaxXmlParser(new File(args[0]));
        List<AfterSaleServer> servers = parser.parse();
        //System.out.println("servers's size is " + servers.size());

        Iterator<AfterSaleServer> iterator = servers.iterator();

        while(iterator.hasNext()) {
            AfterSaleServer server = iterator.next();
            System.out.println(server.getProvice() + "; " + server.getCompany() + "; " + 
                    server.getPhone() + "; " + server.getAddress() + "\n");
        }

    }
}
