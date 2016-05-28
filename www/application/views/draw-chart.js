$(document).ready(function() {
    const history = 10;
    var labels = [];
    var temperatureData = [];
    var airHumidityData = [];
    var soilHumidityData = [];
    var insolationData = [];
    var waterLevelData = [];
    var context = $("#chart_canvas");
    var chartData = {
        labels: labels,
        datasets: [
            {
                label: "Temperatura",
                fill: false,
                lineTension: 0.1,
                backgroundColor: "rgba(75,192,192,0.4)",
                borderColor: "rgba(75,192,192,1)",
                borderCapStyle: 'butt',
                borderDashOffset: 0.0,
                borderJoinStyle: 'miter',
                pointBorderColor: "rgba(75,192,192,1)",
                pointBackgroundColor: "#fff",
                pointBorderWidth: 1,
                pointHoverRadius: 5,
                pointHoverBackgroundColor: "rgba(75,192,192,1)",
                pointHoverBorderColor: "rgba(220,220,220,1)",
                pointHoverBorderWidth: 2,
                pointRadius: 1,
                pointHitRadius: 10,
                data: temperatureData,
            },
            {
                label: "Wilgotność powietrza",
                fill: false,
                lineTension: 0.1,
                backgroundColor: "rgba(210,105,30,0.4)",
                borderColor: "rgba(210,105,30,1)",
                borderCapStyle: 'butt',
                borderDashOffset: 0.0,
                borderJoinStyle: 'miter',
                pointBorderColor: "rgba(210,105,30,1)",
                pointBackgroundColor: "#fff",
                pointBorderWidth: 1,
                pointHoverRadius: 5,
                pointHoverBackgroundColor: "rgba(210,105,30,1)",
                pointHoverBorderColor: "rgba(220,220,220,1)",
                pointHoverBorderWidth: 2,
                pointRadius: 1,
                pointHitRadius: 10,
                data: airHumidityData,
            },
            {
                label: "Wilgotność gleby",
                fill: false,
                lineTension: 0.1,
                backgroundColor: "rgba(220,20,60,0.4)",
                borderColor: "rgba(220,20,60,1)",
                borderCapStyle: 'butt',
                borderDashOffset: 0.0,
                borderJoinStyle: 'miter',
                pointBorderColor: "rgba(220,20,60,1)",
                pointBackgroundColor: "#fff",
                pointBorderWidth: 1,
                pointHoverRadius: 5,
                pointHoverBackgroundColor: "rgba(220,20,60,1)",
                pointHoverBorderColor: "rgba(220,220,220,1)",
                pointHoverBorderWidth: 2,
                pointRadius: 1,
                pointHitRadius: 10,
                data: soilHumidityData,
            },
            {
                label: "Naświetlenie",
                fill: false,
                lineTension: 0.1,
                backgroundColor: "rgba(75,0,130,0.4)",
                borderColor: "rgba(75,0,130,1)",
                borderCapStyle: 'butt',
                borderDashOffset: 0.0,
                borderJoinStyle: 'miter',
                pointBorderColor: "rgba(75,0,130,1)",
                pointBackgroundColor: "#fff",
                pointBorderWidth: 1,
                pointHoverRadius: 5,
                pointHoverBackgroundColor: "rgba(75,0,130,1)",
                pointHoverBorderColor: "rgba(220,220,220,1)",
                pointHoverBorderWidth: 2,
                pointRadius: 1,
                pointHitRadius: 10,
                data: insolationData,
            },
            {
                label: "Poziom wody",
                fill: false,
                lineTension: 0.1,
                backgroundColor: "rgba(34,139,34,0.4)",
                borderColor: "rgba(34,139,34,1)",
                borderCapStyle: 'butt',
                borderDashOffset: 0.0,
                borderJoinStyle: 'miter',
                pointBorderColor: "rgba(34,139,34,1)",
                pointBackgroundColor: "#fff",
                pointBorderWidth: 1,
                pointHoverRadius: 5,
                pointHoverBackgroundColor: "rgba(34,139,34,1)",
                pointHoverBorderColor: "rgba(220,220,220,1)",
                pointHoverBorderWidth: 2,
                pointRadius: 1,
                pointHitRadius: 10,
                data: waterLevelData,
            }
        ]
    };
    $.ajax({
        dataType: "json",
        url: "index.php/watering/showdata",
        success: function(data) {
            for (i = 0; i < history; i++) {
                labels[i] = data[i].date;
                temperatureData[i] = data[i].temperature;
                airHumidityData[i] = data[i].air_humidity;
                soilHumidityData[i] = data[i].soil_humidity;
                insolationData[i] = data[i].insolation;
                waterLevelData[i] = data[i].water_level;
            }
            var lineChart = new Chart(context, {
                type: 'line',
                data: chartData
            });
        }
    });
});
