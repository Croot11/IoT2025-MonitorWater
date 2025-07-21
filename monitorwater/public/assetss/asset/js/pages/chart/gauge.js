
// Zones
zoneGauge = new Gauge(document.getElementById("gauge-zone"));
var opts = {
    angle: -0.25,
    lineWidth: 0.2,
    radiusScale:0.9,
    pointer: {
        length: 0.6,
        strokeWidth: 0.03,
        color: '#000000'
    },
    staticLabels: {
        font: "10px sans-serif",
        labels: [0, 300, 500, 1000],
        fractionDigits: 0
    },
    renderStaticLabels: function (labelValue) {
        if (labelValue === 2100) {
            return "300"; // Hiển thị 300 thay vì 2100
        }
        return labelValue.toString();
    },
    staticZones: [
        // {strokeStyle: "#30B32D", min: 0, max: 200},
        // {strokeStyle: "#30B32D", min: 200, max: 500},
        {strokeStyle: "#30B32D", min: 0, max: 300},
        {strokeStyle: "#FFDD00", min: 300, max: 500},
        {strokeStyle: "#F03E3E", min: 500, max: 1000}
    ],
    limitMax: false,
    limitMin: false,
    highDpiSupport: true
}
zoneGauge.setOptions(opts);
zoneGauge.setTextField(document.getElementById("zone-textfield"));
zoneGauge.minValue = 0;
zoneGauge.maxValue = 1000;
zoneGauge.set(500);

// function updateWaterQuality() {
//     fetch('/get-distance-to-water')  // Đảm bảo API đúng
//         .then(response => response.json())
//         .then(data => {
//             let waterQuality = data.water_quality;
//             document.getElementById('zone-textfield').innerText = waterQuality; // Hiển thị số liệu
//             zoneGauge.set(waterQuality); // Cập nhật Gauge
//         })
//         .catch(error => console.error('Lỗi lấy dữ liệu:', error));
// }

function updateWaterQuality() {
    fetch('/get-distance-to-water')  // Đảm bảo API đúng
        .then(response => response.json())
        .then(data => {
            if (data.device_1) {
                let waterQuality1 = data.device_1.water_quality;
                document.getElementById('zone-textfield').innerText = waterQuality1; // Thiết bị 1
                zoneGauge.set(waterQuality1); // Cập nhật Gauge 1
            }
        })
        .catch(error => console.error('Lỗi lấy dữ liệu:', error));
}
setInterval(updateWaterQuality, 100);

