<?php
defined('BASEPATH') OR exit('No direct script access allowed');
?><!DOCTYPE html>
<html lang="pl-PL">
<head>
	<meta charset="UTF-8">
	<title>Automatyczna stacja podlewania roślin</title>
	<meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="Automatic watering system">
    <meta name="keywords" content="auto,automatic,watering,system">
    <meta name="author" content="Marcin Wojnakowski">
    <link rel="icon" href="<?= base_url("assets/icon.png"); ?>">
	<link rel="stylesheet" href="<?= base_url("assets/css/bootstrap.min.css"); ?>">
	<script src="<?= base_url("assets/js/jquery-1.12.3.min.js"); ?>"></script>
	<script src="<?= base_url("assets/js/bootstrap.min.js"); ?>"></script>
    <script src="<?= base_url("assets/js/Chart.min.js"); ?>"></script>
    <script src="<?= base_url("application/views/draw-chart.js"); ?>"></script>
    <script>
        $(document).ready(function() {
            var isSettings = true;
            var isInfo = true;
            var isChart = true;
            var arduino_ip = "";
            var arduino_port = "";
            var humidity = "";
            var isOn = "0";
            $.ajax({
                dataType: "json",
                url: "index.php/watering/getstatus",
                success: function(data) {
                    isOn = data.isOn;
                    arduino_ip = data.arduino_ip;
                    arduino_port = data.arduino_port;
                    humidity = data.humidity;
                    $("#arduino_ip").val(arduino_ip);
                    $("#arduino_port").val(arduino_port);
                    $("#humidity").val(humidity);
                    if(isOn == "1") {
                        $("#turn").html("Wyłącz");
                        $("#turn").removeClass("btn-success");
                        $("#turn").addClass("btn-danger");
                    } else {
                        $("#turn").html("Włącz");
                        $("#turn").removeClass("btn-danger");
                        $("#turn").addClass("btn-success");
                    }
                }
            });
            updateInfo();
            setInterval(updateInfo, 10 * 60 * 1000);
            $("#arduino_ip").on("change", function() {
                $.ajax({
                    dataType: "json",
                    method: "POST",
                    data: {ip: $("#arduino_ip").val()},
                    url: "index.php/watering/changearduinoip"
                });
            });
            $("#arduino_port").on("change", function() {
                $.ajax({
                    dataType: "json",
                    method: "POST",
                    data: {port: $("#arduino_port").val()},
                    url: "index.php/watering/changearduinoport"
                });
            });
            $("#humidity").on("change", function() {
                $.ajax({
                    dataType: "json",
                    method: "POST",
                    data: {humidity: $("#humidity").val()},
                    url: "index.php/watering/changehumidity"
                });
            });
            $("#turn").on("click", function() {
                if(isOn == "1") {
                    $.ajax({
                        dataType: "json",
                        url: "index.php/watering/stop",
                        success: function(data) {
                            isOn = data.isOn;
                            $("#turn").html("Włącz");
                            $("#turn").removeClass("btn-danger");
                            $("#turn").addClass("btn-success");
                        }
                    });
                    $("#alert").empty();
                } else {
                    $.ajax({
                        dataType: "json",
                        url: "index.php/watering/run",
                        success: function(data) {
                            isOn = data.isOn;
                            $("#turn").html("Wyłącz");
                            $("#turn").removeClass("btn-success");
                            $("#turn").addClass("btn-danger");
                        }
                    });
                    $("#alert").append(
                        "<div class='alert alert-success' role='alert'>"+
                            "<strong>Uruchomiono!</strong> Automatyczne podlewanie zostało załączone"+
                        "</div>"
                    );
                }
            });
            $("#settings").on("click", function() {
                if(isSettings) {
                    isSettings = false;
                    $("#settings_panel").fadeOut();
                } else {
                    isSettings = true;
                    $("#settings_panel").fadeIn();
                }
            });
            $("#info").on("click", function() {
                if(isInfo) {
                    isInfo = false;
                    $("#info_panel").fadeOut();
                } else {
                    isInfo = true;
                    $("#info_panel").fadeIn();
                }
            });
            $("#chart").on("click", function() {
                if(isChart) {
                    isChart = false;
                    $("#chart_panel").fadeOut();
                } else {
                    isChart = true;
                    $("#chart_panel").fadeIn();
                }
            });
        });
        function updateInfo() {
            $.ajax({
                dataType: "json",
                url: "index.php/watering/showCurrentdata",
                success: function(data) {
                    $("#date").html(data.date);
                    $("#temperature").html(data.temperature);
                    $("#air_humidity").html(data.air_humidity);
                    $("#soil_humidity").html(data.soil_humidity);
                    $("#insolation").html(data.insolation);
                    $("#water_level").html(data.water_level);
                }
            });
        }
    </script>
</head>
<body>
    <div class="container">
        <div class="jumbotron">
            <h1>Auto Watering System</h1>
            <p>Aplikacja do podlewania roślin</p>
        </div>
        <div id="alert"></div>
        <div class="row">
            <div class="col-xs-12 col-sm-6 col-md-4">
                <div class="panel panel-info">
                    <div class="panel-heading">Home</div>
                    <div class="panel-body">
                        <div class="btn-group-vertical btn-block">
                            <a class="btn btn-success btn-lg" role="button" id="turn">Włącz</a>
	                        <a class="btn btn-info" role="button" id="settings">
                                <span class="glyphicon glyphicon-wrench" aria-hidden="true"></span> Ustawienia
                            </a>
		                    <a class="btn btn-info" role="button" id="info">
                                <span class="glyphicon glyphicon-eye-open" aria-hidden="true"></span> Informacje
                            </a>
                            <a class="btn btn-info" role="button" id="chart">
                                <span class="glyphicon glyphicon-signal" aria-hidden="true"></span> Wykres
                            </a>
                            <a href="application/logs/sensors.txt" class="btn btn-info" role="button" id="logs">
                                <span class="glyphicon glyphicon-download" aria-hidden="true"></span> Pobierz logi
                            </a>
		                    <a href="http://<?= $_SERVER['SERVER_ADDR']; ?>:3000" class="btn btn-info" role="button">
                                <span class="glyphicon glyphicon-sunglasses" aria-hidden="true"></span> Debugging
                            </a>
                        </div>
                    </div>
                </div>
            </div>
            <div class="col-xs-12 col-sm-6 col-md-4" id="info_panel">
                <div class="panel panel-info">
                    <div class="panel-heading">Aktualne informacje</div>
                    <div class="panel-body">
                        <p>Data: <span id="date"></span></p>
	                    <p>Temperatura: <span id="temperature"></span></p>
	                    <p>Wilgotność powietrza: <span id="air_humidity"></span>%</p>
	                    <p>Wilgotność gleby: <span id="soil_humidity"></span>%</p>
	                    <p>Nasłonecznienie: <span id="insolation"></span> lux</p>
	                    <p>Poziom wody: <span id="water_level"></span>%</p>
                    </div>
                    <div class="panel-footer">Dane są odświerzane co 10 minut</div>
                </div>
            </div>
            <div class="col-xs-12 col-sm-6 col-md-4" id="settings_panel">
                <div class="panel panel-info">
                    <div class="panel-heading">Ustawienia</div>
                    <div class="panel-body">
                        <form role="form">
		                    <div class="form-group">
                                <label for="arduino_ip">Arduino IP:</label>
                                <input type="text" class="form-control" id="arduino_ip">
                            </div>
                            <div class="form-group">
                                <label for="arduino_port">Arduino Port:</label>
                                <input type="text" class="form-control" id="arduino_port">
                            </div>
                            <div class="form-group">
                                <label for="humidity">Poziom wilgotności:</label>
                                <div class="input-group">
                                    <input type="number" class="form-control" id="humidity">
                                    <span class="input-group-addon">+/- 10%</span>
                                </div>
                            </div>
	                    </form> 
                    </div>
                </div>
            </div>
        </div>
        <div class="row">
            <div id="chart_panel">
                <div class="panel panel-info">
                    <div class="panel-heading">Wykres</div>
                    <div class="panel-body">
                        <canvas id="chart_canvas"></canvas>
                    </div>
                </div>
            </div>
        </div>
        <div><p class="text-muted center-block">Auto Watering System 2016</p></div>
    </div>
</body>
</html>
