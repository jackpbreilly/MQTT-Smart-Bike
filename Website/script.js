mapboxgl.accessToken = 'pk.eyJ1IjoiamFja3BicmVpbGx5IiwiYSI6ImNsMHF2M3NsaTJhMDczY3F0b3ltN2NidHcifQ.H7-FHSVZCZG8j3n-ul55vw';
const map = new mapboxgl.Map({
container: 'map',
style: 'mapbox://styles/mapbox/satellite-v9',
zoom: 0
});


const date = new Date();
let init_time = date.getTime();
const data = new Map();

let GPSTime = init_time;
let HRTime = init_time;
let HSTime = init_time;

map.on('load', async () => {
// We fetch the JSON here so that we can parse and use it separately
// from GL JS's use in the added source.
const response = await fetch(
'/a.html'
);
const data = await response.json();
// save full coordinate list for later
const coordinates = data.features[0].geometry.coordinates;
// start by showing just the first coordinate
data.features[0].geometry.coordinates = [coordinates[0]];

//coordinates.push([-3.160785, 56.175913]);

// add it to the map
map.addSource('trace', { type: 'geojson', data: data });
map.addLayer({
'id': 'trace',
'type': 'line',
'source': 'trace',
'paint': {
'line-color': 'yellow',
'line-opacity': 0.75,
'line-width': 5
}
});

// setup the viewport
map.jumpTo({ 'center': coordinates[0], 'zoom': 14 });
map.setPitch(30);

let i = 0;

const intervalGPS = setInterval(function() {
        var oReq = new XMLHttpRequest(); // New request object
        oReq.onload = function() {
            let json_data = this.responseText;
if(json_data != ""){
            const json_data_as_array = JSON.parse(json_data);
            let len_of_data = Object.keys(json_data_as_array[0]).length;
             if (len_of_data > 0){
               GPSTime = parseInt(json_data_as_array[0][len_of_data - 1]['Timestamp']);

               for(let j =0; j < len_of_data; j++){
                 let arr = [(json_data_as_array[0][j][0]), (json_data_as_array[0][j][1])]
               data.features[0].geometry.coordinates.push(arr);
               map.getSource('trace').setData(data);
             }
             }
        }}
oReq.open("get", "dbcall.php?sensor=GPS&timestamp=" + GPSTime, true);
oReq.send();

}, 1000);

const intervalHR = setInterval(function() {
        var oReq = new XMLHttpRequest(); // New request object
        oReq.onload = function() {
            let json_data = this.responseText;
if(json_data != ""){
            const json_data_as_array = JSON.parse(json_data);
            let len_of_data = Object.keys(json_data_as_array[0]).length;
             if (len_of_data > 0){
               HRTime = parseInt(json_data_as_array[0][len_of_data - 1]['Timestamp']);

               for(let j =0; j < len_of_data; j++){
                 document.getElementById('heartrate').innerText =json_data_as_array[0][j]['Heartrate'] + " BPM";
             }
             }
        }}
oReq.open("get", "dbcall.php?sensor=heartrate&timestamp=" + HRTime, true);
oReq.send();

}, 1000);

let stateHS = "0";
let distInCM = 0;

const intervalHS = setInterval(function() {
        var oReq = new XMLHttpRequest(); // New request object
        oReq.onload = function() {
            let json_data = this.responseText;
            if(json_data != ""){
            const json_data_as_array = JSON.parse(json_data);
            let len_of_data = Object.keys(json_data_as_array[0]).length;
             if (len_of_data > 0){
               for(let j =0; j < len_of_data; j++){
                if(json_data_as_array[0][j]['State'] === "1" && stateHS === "0"){
                  distInCM = distInCM +1;
                  HSTime = parseInt(json_data_as_array[0][len_of_data - 1]['Timestamp']);
                  document.getElementById('miles').innerText =distInCM + " Miles";
                }
                stateHS = json_data_as_array[0][j]['State'];
             }
             }
        }}
oReq.open("get", "dbcall.php?sensor=hallsensor&timestamp=" + HSTime, true);
oReq.send();

}, 1000);




const timer = setInterval(() => {
if (i < coordinates.length) {
data.features[0].geometry.coordinates.push(coordinates[i]);
map.getSource('trace').setData(data);
map.panTo(coordinates[i]);
i++;
} else {
window.clearInterval(timer);
}
}, 10);
});






//setTimeout(function(){ coordinates.push([-3.160785, 56.175913]); }, 1000);
