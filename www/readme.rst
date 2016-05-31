A web application to control the auto watering system.
======================================================

Server Requirements
-------------------

PHP version 5.4 or newer is recommended.

MySQL database with 2 tables:
info (
    info_id (int, not null, primary key),
    isOn (tinyint, not null),
    arduino_ip (varchar(15), not null),
    arduino_port (varchar(4), not null),
    humidity (int, not null))
logs (
    date (timestamp, not null, primary key),
    temperature (int, not null),
    air_humidity (int, not null),
    soil_humidity (int, not null),
    insolation (int, not null),
    water_level (int, not null))

Acknowledgement
---------------

The application bases on CodeIgniter framework and uses Bootstrap for styling.
