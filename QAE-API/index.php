<?php
//! ProjetQAE-API (c) Daniel THIRION & Antonin ESTELLE
//! Licensed under LGPLv3. A copy of the license should have been given
//! with the software. If not, find it online.
//? -------------------------------------------------------------------
//? File Name: index.php
//? Author: Daniel THIRION
//? Co-author: Antonin ESTELLE
//? Supervision: M. CONTARET, IUT GEII Salon de Provence
//? Last modified: 3/11/2019
//? -------------------------------------------------------------------
//* Description
// Part of ProjetQAE : Using Arduino, gas sensors, a RPi and a web server,
// Monitor and display the current air quality.
// More info at https://github.com/defvs/ProjetQAE
//
// This part of the project corresponds to the API running on a webserver.
// It allows the IoT device to send in informations, and user webpages
// to request them.
// Information is stored in a MySQL database, present next to this file as "qae.sql".

$date = date('Y-m-d H:i:s');                                            //? Current date and time formatted for MySQL
$sql = mysqli_connect('localhost', 'root', 'password', 'qae2', '3306');          //? Connection to the MySQL database.

if ($_SERVER['REQUEST_METHOD'] === 'POST'){ //! POST REQUEST (sending in data)
    $data = file_get_contents('php://input');                               //? Retreive file sent via POST request
    $json = json_decode($data);                                             //? Deserialization to object from JSON

    //* Retreival of row counts from database
    $result = mysqli_query($sql, "SELECT * FROM data_numeric");
    $values_numeric_count = mysqli_field_count($sql) - 2;
    mysqli_free_result($result);
    $result = mysqli_query($sql, "SELECT * FROM data_analog");
    $values_analog_count = mysqli_field_count($sql) - 2;
    mysqli_free_result($result);

    if (!isset($json->password)){               //? Password presence check
        http_response_code(400);
        echo "Password missing";
    }else if (!isset($json->sender)) {           //? Sender name presence check
        http_response_code(400);
        echo "Sender missing";
    }else if (!isset($json->values_numeric)){            //? Numerical values presence check
        http_response_code(400);
        echo "values_numeric missing";
    }else if (count($json->values_numeric) != $values_numeric_count){         //? Numerical values count check
        http_response_code(400);
        echo "Wrong values_numeric count";
    }else if (!isset($json->values_analog)){            //? Analog values presence check
        http_response_code(400);
        echo "values_analog missing";
    }else if (count($json->values_analog) != $values_analog_count){         //? Analog values count check
        http_response_code(400);
        echo "Wrong values_analog count";
    }

    else if($json->password == "password"){     //? Password correspondence check
        $error = false;

        $flat = implode(",", $json->values_numeric);
        $query = "INSERT INTO data_numeric VALUES (0,$flat,'$date');";
        $error = !mysqli_query($sql, $query);

        $flat = implode(",", $json->values_analog);
        $query = "INSERT INTO data_analog VALUES (0,$flat,'$date');";
        $error = !mysqli_query($sql, $query);

        if ($error >= 1) {
            http_response_code(500);
            echo "Database error";
        }else{
            http_response_code(200);
            echo "OK";
        }
    }else{
        http_response_code(400);
        echo "Wrong password";  
    }
}else if ($_SERVER['REQUEST_METHOD'] === 'GET'){ //! GET REQUEST (requesting data)
    if (isset($_GET['limit'])){
        $limit = $_GET['limit'];
    } else {
        $limit = 1;
    }
    $data = new \stdClass(); //* Data object which will be converted to JSON

    //* SQL queries for numeric and analog values
    $query = "SELECT * FROM data_numeric ORDER BY id DESC LIMIT $limit";
    $result = mysqli_query($sql, $query);

    $i = 0;
    while ($row = mysqli_fetch_assoc($result)){
        unset($row['id']);
        $data->$i->numeric = $row;
        $i++;
    }

    mysqli_free_result($result);


    $query = "SELECT * FROM data_analog ORDER BY id DESC LIMIT $limit";
    $result = mysqli_query($sql, $query);
    
    $i = 0;
    while ($row = mysqli_fetch_assoc($result)){
        unset($row['id']);
        $data->$i->analog = $row;
        $i++;
    }

    mysqli_free_result($result);
    
    //? JSON serialization and transmission
    header('Content-Type: application/json');
    http_response_code(200);
    echo json_encode($data);
}   
//? Close database connection
mysqli_close($sql);
