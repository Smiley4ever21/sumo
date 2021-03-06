
package de.tudresden.ws;

import java.net.MalformedURLException;
import java.net.URL;
import javax.xml.namespace.QName;
import javax.xml.ws.Service;
import javax.xml.ws.WebEndpoint;
import javax.xml.ws.WebServiceClient;
import javax.xml.ws.WebServiceException;
import javax.xml.ws.WebServiceFeature;


/**
 * This class was generated by the JAX-WS RI.
 * JAX-WS RI 2.2.9-b130926.1035
 * Generated source version: 2.2
 * 
 */
@WebServiceClient(name = "TraasWS", targetNamespace = "http://ws.tudresden.de/", wsdlLocation = "http://127.0.0.1:8080/TRAAS_WS?wsdl")
public class TraasWS
    extends Service
{

    private final static URL TRAASWS_WSDL_LOCATION;
    private final static WebServiceException TRAASWS_EXCEPTION;
    private final static QName TRAASWS_QNAME = new QName("http://ws.tudresden.de/", "TraasWS");

    static {
        URL url = null;
        WebServiceException e = null;
        try {
            url = new URL("http://127.0.0.1:8080/TRAAS_WS?wsdl");
        } catch (MalformedURLException ex) {
            e = new WebServiceException(ex);
        }
        TRAASWS_WSDL_LOCATION = url;
        TRAASWS_EXCEPTION = e;
    }

    public TraasWS() {
        super(__getWsdlLocation(), TRAASWS_QNAME);
    }

    public TraasWS(WebServiceFeature... features) {
        super(__getWsdlLocation(), TRAASWS_QNAME, features);
    }

    public TraasWS(URL wsdlLocation) {
        super(wsdlLocation, TRAASWS_QNAME);
    }

    public TraasWS(URL wsdlLocation, WebServiceFeature... features) {
        super(wsdlLocation, TRAASWS_QNAME, features);
    }

    public TraasWS(URL wsdlLocation, QName serviceName) {
        super(wsdlLocation, serviceName);
    }

    public TraasWS(URL wsdlLocation, QName serviceName, WebServiceFeature... features) {
        super(wsdlLocation, serviceName, features);
    }

    /**
     * 
     * @return
     *     returns ServiceImpl
     */
    @WebEndpoint(name = "ServiceImplPort")
    public ServiceImpl getServiceImplPort() {
        return super.getPort(new QName("http://ws.tudresden.de/", "ServiceImplPort"), ServiceImpl.class);
    }

    /**
     * 
     * @param features
     *     A list of {@link javax.xml.ws.WebServiceFeature} to configure on the proxy.  Supported features not in the <code>features</code> parameter will have their default values.
     * @return
     *     returns ServiceImpl
     */
    @WebEndpoint(name = "ServiceImplPort")
    public ServiceImpl getServiceImplPort(WebServiceFeature... features) {
        return super.getPort(new QName("http://ws.tudresden.de/", "ServiceImplPort"), ServiceImpl.class, features);
    }

    private static URL __getWsdlLocation() {
        if (TRAASWS_EXCEPTION!= null) {
            throw TRAASWS_EXCEPTION;
        }
        return TRAASWS_WSDL_LOCATION;
    }

}
