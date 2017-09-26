//----------------------------------------------------------------------------
//
//  JavaScript side of Project 20.1 for "Learning C with Pebble"
//  Uses PebbleKit.js
//
//  Mike Jipping, August, 2016

var randomURL = "https://www.random.org/integers/?num=1&min=1&max=50&col=1&base=10&format=plain&rnd=new";

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
});