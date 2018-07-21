
var colors = ['#f45941', '#f4f142', '#333333', '#c3e6cb', '#dc3545', '#6c757d'];

/* large line chart */
var chLine = document.getElementById("lineChart");
var chartData = {
    labels: ["17 Jul", "18 Jul", "19 Jul", "20 Jul", "21 Jul", "22 Jul", "23 Jul"],
    datasets: [{
        data: [09.00, 11.00, 10.00, 10.30, 09.30, 11.00, 10.00],
        backgroundColor: 'transparent',
        borderColor: colors[0],
        borderWidth: 4,
        pointBackgroundColor: colors[0]
    }]
};

if (chLine) {
    new Chart(chLine, {
        type: 'line',
        data: chartData,
        options: {
            scales: {
                yAxes: [{
                    ticks: {
                        beginAtZero: false
                    }
                }]
            },
            legend: {
                display: false
            }
        }
    });
}

