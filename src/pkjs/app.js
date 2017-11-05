//----------------------------------------------------------------------------
//
//  JavaScript side of Project 20.1 for "Learning C with Pebble"
//  Uses PebbleKit.js
//
//  Mike Jipping, August, 2016

var randomURL = "https://www.random.org/integers/?num=1&min=1&max=50&col=1&base=10&format=plain&rnd=new";
var myAPIKey = 'fd541b64f6da378322b01f9a122d5dee';
var APIURL = 'http://api.openweathermap.org/data/2.5/weather';
var watchID;

// Called when the message send attempt succeeds
function messageSuccessHandler() {
  console.log("Message send succeeded.");  
}

// Called when the message send attempt fails
function messageFailureHandler() {
  console.log("Message send failed.");
}

// Gets a random number between 1 and 50
function get_random() {
   var req = new XMLHttpRequest();  
   req.open('GET', randomURL, false);   
   req.send(null);  
   if(req.status != 200) return "0";
    
   var page = req.responseText;

  return page;
}

function getLocation() {
  watchID = navigator.geolocation.getCurrentPosition(getPlaceName, error, options);
}

 var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function getPlaceName(pos) {
  
  // Construct URL
  var url = APIURL + '?' + 'lat=' + pos.coords.latitude + '&lon=' + pos.coords.longitude + '&cnt=1&appid=' + myAPIKey;
  console.log("URL=" + url);
  // Send request
  xhrRequest(url, 'GET',
    function(responseText) {
      // responseText contains a JSON object with city info
      var json = JSON.parse(responseText);
      console.log("Response text = " + responseText);
      var city = json.name;
      console.log("CITY retrieved = " + city);
      Pebble.sendAppMessage({"CITY": city}, messageSuccessHandler, messageFailureHandler);
    });
}
  
function error() {  
}

function options() {
  
}

// Called when JavaScript is ready
Pebble.addEventListener("ready", function(e) {
  console.log("JS is ready!");
});
												
// Called when incoming message from the Pebble is received
Pebble.addEventListener("appmessage", function(e) {
  var rand;
  var dictionary = e.payload;
    
  console.log('Got message: ' + JSON.stringify(dictionary));
  if (dictionary['ASK']) {
        rand = get_random();
        console.log("Number = " + rand);
        Pebble.sendAppMessage({"RANDOM": rand}, messageSuccessHandler, messageFailureHandler);
  }
  if (dictionary['CITY']) {
        getLocation();
  }
  
  if (dictionary['LOCATION_LAT'] && dictionary['LOCATION_LON']) {
        console.log("Got location lat" + dictionary['LOCATION_LAT']);
        console.log("Got location lon" + dictionary['LOCATION_LON']);
  }
    
});