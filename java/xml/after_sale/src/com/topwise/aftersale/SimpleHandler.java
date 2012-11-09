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

import java.util.ArrayList;
import java.util.List;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class SimpleHandler extends DefaultHandler {
    private List<AfterSaleServer> servers;
    private boolean isInServers = false;
    private final static String H_AFTER_SERVERS_NAME = "aftersale_servers";
    private final static String H_ITEM_NAME = "item";
    private final static String PROVICE = "provice";
    private final static String COMPANY = "company";
    private final static String ADDRESS = "address";
    private final static String PHONE = "phone";

    public List<AfterSaleServer> getServers() {
        return servers;
    }

    @Override
    public void startDocument() throws SAXException {
        super.startDocument();
        servers = new ArrayList<AfterSaleServer>();
    }

    @Override
    public void startElement(String uri, String localName, String name,
            Attributes attributes) throws SAXException {
        super.startElement(uri, localName, name, attributes);
        //System.out.println("in startElement name is " + localName);
        if (name.equalsIgnoreCase(H_AFTER_SERVERS_NAME)) {
           // System.out.println("get headers\n");
            isInServers = true;
            return;
        }

        if (name.equalsIgnoreCase(H_ITEM_NAME) && isInServers == true) {
            //System.out.println("in startElement\n");
            AfterSaleServer server = new AfterSaleServer();
            server.setProvice(attributes.getValue(PROVICE));
            server.setCompany(attributes.getValue(COMPANY));
            server.setPhone(attributes.getValue(PHONE));
            server.setAddress(attributes.getValue(ADDRESS));
            servers.add(server);
        } else {
            isInServers = false;
        }

        return;
    }

}
