<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

$timestamp=$_GET['timestamp'];


if ($_GET["sensor"] == "hallsensor"){
 $sql ="SELECT * from Hall_Sensor WHERE Hall_Sensor_Time > '$timestamp'";

}
elseif ($_GET["sensor"] == "GPS"){
 $sql ="SELECT * from GPS WHERE GPS_Time > '$timestamp'";

}elseif ($_GET["sensor"] == "heartrate"){
 $sql ="SELECT * from Heart_Rate_Sensor WHERE Heart_Rate_Sensor_Time > '$timestamp'";

}

class MyDB extends SQLite3 {
      function __construct() {
         $this->open('sensor.db');
      }
   }

   $db = new MyDB();
   if(!$db) {
   } else {
   }

$all_data = array();

$all_data[0] = array();

   $ret = $db->query($sql);
       while ($row = $ret->fetchArray(SQLITE3_ASSOC)) {



if ($_GET["sensor"] == "hallsensor"){
$all_data[0][] = array(
  "Timestamp" => $row["Hall_Sensor_Time"],
  "State" => $row["Hall_Sensor_State"]
);}

elseif ($_GET["sensor"] == "GPS"){
$all_data[0][] = array(
 floatval( $row["GPS_Position_2"]), floatval($row["GPS_Position"]), "Timestamp" => $row["GPS_Time"]
);
   }elseif ($_GET["sensor"] == "heartrate"){
$all_data[0][] = array(
  "Timestamp" => $row["Heart_Rate_Sensor_Time"],
  "Heartrate" => $row["Heart_Rate_Sensor_Heart_Rate"]
);}

}

   $db->close();

echo json_encode($all_data);
?>
