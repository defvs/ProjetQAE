<?php
//! ProjetQAE-API (c) Daniel THIRION & Antonin ESTELLE
//! Licensed under LGPLv3. A copy of the license should have been given
//! with the software. If not, find it online.
//? -------------------------------------------------------------------
//? File Name: main.cpp
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

$date = date('Y-m-d H:i:s');                                            //! Date et heure actuelle
$sql = mysqli_connect('localhost', 'root', '', 'qae', '3306');          //! Connexion à la base de données

if ($_SERVER['REQUEST_METHOD'] === 'POST'){
    $data = file_get_contents('php://input');                               //! Récupération de la trame
    $json = json_decode($data);                                             //! Traduction de la trame reçue

    $result = mysqli_query("SELECT * FROM data_numeric");
    $values_numeric_count = mysqli_num_rows($result) - 1;
    mysqli_free_result($result);
    $result = mysqli_query("SELECT * FROM data_analog");
    $values_analog_count = mysqli_num_rows($result) - 1;
    mysqli_free_result($result);

    if (!isset($json->password)){               //! Vérification de la présence mot de passe
        http_response_code(400);
        echo "mot de passe non envoyé";
    }
    else if (!isset($json->sender)) {           //! Vérification de la présence du nom de l'envoyeur
        http_response_code(400);
        echo "sender non envoyé";
    }
    else if (!isset($json->values_numeric)){            //! Vérification des valeurs numériques
        http_response_code(400);
        echo "values_numeric non envoyé";
    }
    else if (count($json->values_numeric) != $values_numeric_count){         //! Vérification du nombre de valeurs numériques reçue
        http_response_code(400);
        echo "trop / pas assez de valeurs";
    }
    else if (!isset($json->values_analog)){            //! Vérification des valeurs analogiques
        http_response_code(400);
        echo "values_analog non envoyé";
    }
    else if (count($json->values_analog) != $values_analog_count){         //! Vérification du nombre de valeurs analogiques reçue
        http_response_code(400);
        echo "trop / pas assez de valeurs";
    }

    else if($json->password == "password"){     //! Vérification de la bonne orthographe du mot de passe
        $error = false;
        for ($i = 0; $i < $values_numeric_count; $i++) { 
            if ($json->values_numeric[$i] == -1) {
                continue;
            }
            $a = $i + 1;
            $val = $json->values_numeric[$i];
            $str = "UPDATE data_numeric SET value = $val, last_update = '$date' WHERE data_numeric.index = $a";

            $error = !mysqli_query($sql, $str)
        }
        for ($i = 0; $i < 2; $i++) { 
            if ($json->values_analog[$i] == -1) {
                continue;
            }
            $a = $i + 1;
            $val = $json->values_analog[$i];
            $str = "UPDATE data_analog SET value = $val, last_update = '$date' WHERE data_analog.index = $a";

            $error = !mysqli_query($sql, $str)
        }
        if ($error >= 1) {
            http_response_code(500);
            echo "erreur bdd";
        }
        else{
            http_response_code(200);
            echo "OK";
        }
    }
    else{
        http_response_code(400);
        echo "mauvais password";  
    }
}
else if ($_SERVER['REQUEST_METHOD'] === 'GET'){
    $data = new \stdClass();
    $str = "SELECT name, value, last_update FROM data_numeric WHERE 1";
    $result = mysqli_query($sql, $str);
    if ($result->num_rows === 7){
        $data -> values_numeric = $result->fetch_all();  
    }
    else{
        http_responde_code(500);
        print "erreur bdd";
        exit(0);
    }

    $str = "SELECT name, value, last_update FROM data_analog WHERE 1";
    $result = mysqli_query($sql, $str);
    if ($result->num_rows === 2){
        $data -> values_analog = $result->fetch_all();
    }
    mysqli_free_result($result);
    else{
        http_responde_code(500);
        print "erreur bdd";
        exit(0);
    }
    header('Content-Type: application/json');
    http_response_code(200);
    echo json_encode($data);   
}
mysqli_close($sql);
?>