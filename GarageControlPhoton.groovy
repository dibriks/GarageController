/**
 * garage contact from photon
 *  
Copyright 2014 SmartThings
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 *  in compliance with the License. You may obtain a copy of the License at:
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed
 *  on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License
 *  for the specific language governing permissions and limitations under the License.
 *
 */
  
// user input preferences  
  preferences {
    input name: "deviceId", type: "text", title: "Device ID", required: true
    input name: "token", type: "password", title: "Access Token", required: true
    input name: "contactVar", type: "text", title: "Contact Variable Name", required: true, defaultValue: "GarageStatus"
}

// name the capabilities app author etc
 metadata {
	definition (name: "Garage Door", namespace: "GD", author: "bscuderi13") {      
        // capability "actuator"
        capability "contact sensor"
        capability "Switch"
        capability "Polling"
        capability "Sensor"
        capability "Refresh"
        
        attribute "Status", "string"  // Defines a container to hold the state of your garage door
     
        command "garageRelay"  // Defines a command to trigger the garage door we want opened
         }

// make the tiles for the device handler		
	tiles {
		standardTile("contact", "device.contact", width: 2, height: 2) {
			state "Open",   label: '${name}', action: "garageRelay", icon: "st.Transportation.transportation12",   backgroundColor: "#44b621"
			state "Closed", label: '${name}', action: "garageRelay",icon: "st.Transportation.transportation14", backgroundColor: "#bc2323	"
			state "In motion", label: '${name}', action: "garageRelay", icon: "st.Transportation.transportation13", backgroundColor: "##e86d13"
            state "Ventilation", label: '${name}', action: "garageRelay",icon: "st.Weather.weather10", backgroundColor: "#153591"
		}
        standardTile("refresh", "device.refresh", inactiveLabel: false, decoration: "flat", width: 1, height: 1) {
            state "", action:"refresh.refresh", icon:"st.secondary.refresh"
        }
		
		main(["contact"])										
		details(["contact", "refresh"])						
	}
    
    
}
// Refresh value

def refresh() {
    log.debug "Executing 'refresh'"
//execute get value command on refresh
    getvalue() 
}
// Get value of contact sensor variable from particle
private getvalue() {
    def closure = { response ->
        log.debug "Contact request was successful, $response.data"
        // This is the line that sends the contact sensors response value of either open or closed to the contact sensor
        sendEvent(name: "contact", value: response.data.result)
        }
// part that gets sent to particle to get the value of the variable
    httpGet("https://api.particle.io/v1/devices/${deviceId}/${contactVar}?access_token=${token}", closure)
}

def contact() {  // This is what happens when tile contact is pushed.
	log.debug "Executing 'garage1'"
	put 'open'  // Calls put(garageRelay) and sends it a "pokreni_otvaranje" which tells the Particle sketch to run GarageRelay with command == open
}


/////////////////////////////////////////////////////////////
def garageRelay() {  // This is what happens when tile g1Status is pushed.
	log.debug "Executing 'garage1'"
	put 'open'  // Calls put(GarageRelay) and sends it a "1" which tells the Particle sketch to run GarageRelay with command == open
}




// How we push commands from the device handler to the Particle
private put(garageRelay) {
	log.debug "sending post";
		httpPost(
			uri: "https://api.particle.io/v1/devices/${deviceId}/GarageRelay",
	        body: [access_token: token, command: garageRelay],  
		) {response -> log.debug (response.data)}
	log.debug "post sent";
    }
