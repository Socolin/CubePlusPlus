<?php
    // Connexion
    $mysqli = new mysqli("localhost", "root", "*****", "mcpp");
    if ($mysqli->connect_errno) {
    echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
        return;
    }
    $mysqli->set_charset("utf8");
?>
