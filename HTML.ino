
void HTML() {

  INDEX_HTML = R"rawliteral(
<html>
<center>
<font size=6>
ESP8266 Rover Menu<br><br>
<a href=/update>OTA Update</a><br>
<a href=/Battery>Battery</a><br>
<a href=/Remote>Remote</a><br><br>
<a href=/Sleep>Sleep</a>
</html>
)rawliteral"; 


REMOTE_HTML = R"rawliteral(
<html>
<center>
<head>

  <script>
  var timer = 0;
  var newWidth = 0;
  var sum, avg = 0;
  var voltageAverage = ["1", "1", "1", "1", "1", "1", "1", "1", "1", "1"];

  
var connection = new WebSocket('ws://192.168.1.18:81/', ['arduino']);
connection.onopen = function () {
  connection.send('Connect ' + new Date());
};
connection.onerror = function (error) {
  console.log('WebSocket Error ', error);
};
connection.onmessage = function (e) {
  console.log('Server: ', e.data);
  newVoltage = (e.data*100);
  voltageAverage.unshift(newVoltage);
  voltageAverage.pop();
  sum = voltageAverage.reduce(function(a, b) { return a + b; });
  avg = sum / 1000;
  newWidth = 100 - ((sum / 10 - 325) * 1.05);
  console.log(newWidth);
//  console.log(avg);

if (newWidth < 1) { 
  newWidth == 100 - ((newVoltage / 10 - 325) * 1.05) + "%";
//  console.log("test");
}

if(avg > 5) { 
displayVoltage = newVoltage/100; 
} else { 
displayVoltage = avg; 
}

  document.getElementById("batteryText").innerHTML = "Battery: " + displayVoltage.toFixed(2) + "v";
  document.getElementById('batteryCell').style.width=newWidth + "%";
};
connection.onclose = function () {
  console.log('WebSocket connection closed');
};

function Stop () {
  connection.send('N'+0);
  console.log('N'+0);
}

function Drive (Dir) {
  connection.send('D'+Dir);
  console.log('D'+Dir);
}


function UpdateRPM () {
  
  var rpm = document.getElementById('rpm').value;
  document.getElementById("p1").innerHTML = rpm;
  connection.send('R'+rpm);
  console.log('R'+rpm);
}

function UpdateServo () {
  var deg = document.getElementById('deg').value;
//  document.getElementById("p1").innerHTML = rpm;
  connection.send('S'+deg);
  console.log('S'+deg);
}

function CameraPower (state) {
  if (state == "2") {
  document.getElementById("p2").innerHTML = "<b>Camera</b><img src=./stream width=100%>";
  }
  else { 
  document.getElementById("p2").innerHTML = "<b>Camera Off</b>";
  }
  connection.send('C'+state);
  console.log('C'+state);
}

function LEDPower (state) {
  if (state == "2") {
  document.getElementById("p3").innerHTML = "LED On";
  }
  else { 
  document.getElementById("p3").innerHTML = "LED Off";
  }
  
  connection.send('L'+state);
  console.log('L'+state);
}

function UpdateStep (state) {
  if (state == "2") {
  document.getElementById("p4").innerHTML = "Full Step";
  }
  else { 
  document.getElementById("p4").innerHTML = "Half Step";
  }
  connection.send('F'+state);
  console.log('F'+state);
}

function mouseupfunc() {
  clearInterval(timer);
  connection.send('N'+0);
  console.log('N'+0);
}

function mousedownfunc(Dir) {
  timer = setInterval(Drive, 10000, Dir);
}


  </script>
</head>

<style>

#batteryContainer {
   position: relative;
   width: 80%
   ;height: 40;
   border: 2px solid black;   
  border-radius: 4px;
   z-index: 0;
   background-image: linear-gradient(to right, #e0622c 0%,#f1ff30 25%,#a5ea25 51%,#73ff54);  
}

#batteryText {
  border: 0px solid #fff; 
  z-index: 2;
  height: 100%;
  display: flex;
  justify-content:center;
  align-content:center;
  flex-direction:column;
}

#batteryCell {  
  position: absolute;
  top: 0;
  right: 0;
  height: 100%;
  z-index: -3;
  border: 0px solid #fff; 
  background-color: #bcbcbc; float:right;
}


.button {
    margin: 4px 2px;
    padding: 8px 16px;
    background-color: white;
    color: black;
    border: 2px solid #555555;
}

.button:hover {
    background-color: #555555;
    color: white;
}

.button2 {
   margin: 2px 0px;
//  padding: 10px 12px;
width: 56px;
height: 56px;
    background-color: #555555;
    color: #d6d8db;
    border: 2px solid #555555;
  border-radius: 4px;
  font-size: 24px;
  background-image: linear-gradient(#555555,#242444);
}

.button2:hover {
    background-color: #555555;
    color: white;
  background-image: linear-gradient(#555555,#576767);
}


body {
  background-color: #bcbcbc;
}

.divTable{
  display: table;
  width: 100%;
}
.divTableRow {
  display: table-row;
}
.divTableHeading {
  background-color: #EEE;
  display: table-header-group;
}
.divTableCell, .divTableHead {
  border: 0px solid #999999;
  display: table-cell;
  margin: 0;
  padding: 0;
}
.divTableHeading {
  background-color: #EEE;
  display: table-header-group;
  font-weight: bold;
}
.divTableFoot {
  background-color: #EEE;
  display: table-footer-group;
  font-weight: bold;
}
.divTableBody {
  display: table-row-group;
}


.slidecontainer {
    width: 100%;
}

.slider {
    -webkit-appearance: none;
    width: 70%;
    height: 5%;
    background: #d3d3d3;
    outline: none;
    opacity: 0.7;
    -webkit-transition: .2s;
    transition: opacity .2s;
}

.slider:hover {
    opacity: 1;
}

.slider::-webkit-slider-thumb {
    -webkit-appearance: none;
    appearance: none;
    width: 5%;
    height: 100%;
    background: #637189;
    cursor: pointer;
}

.slider::-moz-range-thumb {
    width: 5%;
    height: 100%;
    background: #4CAF50;
    cursor: pointer;
}
</style>


<font size=5>Rover Remote Control Interface<br>
<a href=./>Menu</a><br><br>
 <fieldset>
<div class="divTable" style="width: 70%;height: 50%;border: 1px solid #fff;" >
<div class="divTableBody">
<div class="divTableRow">
<div class="divTableCell" style="background: #000000; color: white;"><span id="p2"><b>Camera Off</b></span></div>
<div class="divTableCell" style="width: 280px;"><br><b>Drive</b><br><br>

<div class="divTable" style="width: 150px;border: 0px solid #000;" >
<div class="divTableBody">
<div class="divTableRow">
<div class="divTableCell">&nbsp;</div>
<div class="divTableCell"><input class="button2" id="up" type="button" onclick="Drive(2);" value="&#9650;"> </div>
<div class="divTableCell">&nbsp;</div>
</div>
<div class="divTableRow">
<div class="divTableCell"><input class="button2" id="left" type="button" onclick="Drive(4);" value="&#9664;"></div>
<div class="divTableCell"><input class="button2" id="stop" type="button" onclick="Stop();" value="&#9635;"></div>
<div class="divTableCell"><input class="button2" id="right" type="button" onclick="Drive(1);" value="&#9654;"></div>
</div>
<div class="divTableRow">
<div class="divTableCell">&nbsp;</div>
<div class="divTableCell" style="padding: 5px 5px;"><input class="button2" id="back" type="button" onclick="Drive(3);" value="&#9660;"></div>
<div class="divTableCell">&nbsp;</div>
</div>
</div>
</div><br>
<input class="button" id="camPower" type="button" onclick="CameraPower(2);" value="Camera On"> 
<input class="button" id="camPower" type="button" onclick="CameraPower(1);" value="Camera Off"><br>

<input class="button" id="ledPower" type="button" onclick="LEDPower(2);" value="Night Vision On"> 
<input class="button" id="ledPower" type="button" onclick="LEDPower(1);" value="Night Vision Off"><br>
<span id="p3"></span><br>

<input class="button" id="step" type="button" onclick="UpdateStep(2);" value="Full Step"> 
<input class="button" id="step" type="button" onclick="UpdateStep(1);" value="Half Step"><br>

<span id="p4"></span><br>

<div id="batteryContainer" align="center"><div id="batteryCell"></div><div id="batteryText">Battery: 0.00v</div><br>
</div><br>



</div>
</div>
</div>
</div>

<br>

Speed: <span id="p1">1</span> RPM<br>
<input class="slider" id="rpm" type="range" min="1" max="15" step="1" oninput="UpdateRPM();" value="1" id="RPMRange"></div><br>
Camera Pan<br>
<input class="slider" id="deg" type="range" min="20" max="140" step="1" oninput="UpdateServo();" value="90"><br><br>
</fieldset>
<br><br>
Steps<br>
<a href=/Remote?steps=500>500</a> <a href=/Remote?steps=1000>1000</a> <a href=/Remote?steps=2000>2000</a> <a href=/Remote?steps=5000>5000</a> <a href=/Remote?steps=10000>10000</a> <br>


<a href=/Remote?fullstep=2>Full Step</a> <a href=/Remote?fullstep=1>Half Step</a>
<br><br>
</html>

)rawliteral"; 

}

