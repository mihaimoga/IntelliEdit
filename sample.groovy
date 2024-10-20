import groovy.json.JsonSlurper;
import groovy.json.JsonOutput;

def  linearFind(map, elementToSearch) {
def returnValue = -1;
	map.each { k, v ->    
        if (v.ChildDepartmentID.toString() == elementToSearch.xDEPARTMANI.toString()){
            returnValue = v;
        }
    }
    return returnValue;
}

##//Gets response from Service1
String requestX = "http://<SERVICE_IP>:30080/apigateway/xpath";
URL urlX = new URL( requestX );
HttpURLConnection connX= (HttpURLConnection) urlX.openConnection();
connX.setDoOutput(true);
connX.setRequestMethod("GET");
connX.setRequestProperty("Content-Type", "application/json");
connX.setUseCaches(false);
connX.setConnectTimeout(60000)
def originalBody = connX.inputStream.withCloseable { inStream -> new JsonSlurper().parse( inStream as InputStream ) }

##//Gets response from Service2
String request = "http://<SERVICE_IP>:30080/apigateway/test/test";
URL url = new URL( request );
HttpURLConnection conn= (HttpURLConnection) url.openConnection();
conn.setDoOutput(true);
conn.setRequestMethod("POST");
conn.setRequestProperty("Content-Type", "application/json");
conn.setUseCaches(false);
conn.setConnectTimeout(10000)
def childBody = conn.inputStream.withCloseable { inStream -> new JsonSlurper().parse( inStream as InputStream ) }

##//Compares data from 2 services
##//if it finds a match, it adds the relevant values to the record
originalBody.eachWithIndex { val, idx ->  
	def result=linearFind(childBody, val);
	if(result!=-1){
		val.put("ParentDepartmentID", result.ParentDepartmentID)
		val.put("ParentDepartmentName", result.ParentDepartmentName)
	} 
}

//Returns the result in json format
responseBodyTextToClient=JsonOutput.toJson(originalBody);
