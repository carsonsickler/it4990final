Vue.createApp({
  data() {
    return {
      serviceId: "4fafc201-1fb5-459e-8fcc-c5c9c331914b",
      characteristicId: "beb5483e-36e1-4688-b7f5-ea07361b26a8",
      myCharacteristic: null,
      dec: new TextDecoder(),
      enc: new TextEncoder(),
      message: null,
      moisture: null,
      temperature: null,
      humidity: null,
      justWatered: null,
      lastWatered: null,
      resevoir: null,
      light: null,
      plantName: null,
    };
  },
  methods: {
    namePlant() {
      var name = document.getElementById("plantName").value;
      console.log(name);
      this.plantName = name;
    },
    connectToDevice() {
      navigator.bluetooth.requestDevice({
        filters: [{ services: [this.serviceId] }]
      }).then(device => {
        return device.gatt.connect();
      }).then(server => {
        return server.getPrimaryService(this.serviceId);
      }).then(service => {
        return service.getCharacteristic(this.characteristicId);
      }).then(characteristic => {
        this.myCharacteristic = characteristic;
        alert("Successfully connected to the device!");
      }).catch(error => {
        alert("Error connecting to the device: " + error);
    });
    },
    receiveData() {
      if (!this.myCharacteristic) {
        return;
      }

      // Write a request to the characteristic
      const requestData = this.enc.encode("update");
      this.myCharacteristic.writeValue(requestData).then(() => {
        // After writing the request, read the value
        return this.myCharacteristic.readValue();
      }).then(v => {
        v = this.dec.decode(v);
        console.log(v);

        const moistureMatch = v.match(/MOISTURE: (\d+)/);
        const temperatureMatch = v.match(/TEMPERATURE: (\d+\.\d+)/);
        const humidityMatch = v.match(/HUMIDITY: (\d+\.\d+)/);
        const justWateredMatch = v.match(/JUSTWATERED: (\d+)/);
        const lightMatch = v.match(/LIGHT: (\d+\.\d+)/);
        const resevoirMatch = v.match(/RESEVOIR: (\d+)/);

        this.moisture = moistureMatch ? parseInt(moistureMatch[1]) : 0;
        this.temperature = temperatureMatch ? parseFloat(temperatureMatch[1]) : 0;
        this.humidity = humidityMatch ? parseFloat(humidityMatch[1]) : 0;
        this.justWatered = justWateredMatch ? parseInt(justWateredMatch[1]) : 0;
        this.light = lightMatch ? parseFloat(lightMatch[1]) : 0;
        this.resevoir = resevoirMatch ? parseInt(resevoirMatch[1]) : 0;
        if (!this.justWatered) {
          this.justWatered = true;
          const currentDateAndTime = new Date();

          // Format the date and time
          this.lastWatered = currentDateAndTime.toLocaleString('en-US', { 
          year: 'numeric',
          month: '2-digit',
          day: '2-digit',
          hour: '2-digit',
          minute: '2-digit',
          second: '2-digit',
          hour12: false, // Use 24-hour format
          timeZoneName: 'short'
        });
        }
        if (this.resevoir == 0){
          this.resevoir = 'Empty';
        } else {
          this.resevoir = 'Full';
        }
        console.log("Moisture:", this.moisture);
        console.log("Temperature:", this.temperature);
        console.log("Humidity:", this.humidity);
        console.log("Just Watered:", this.justWatered);
        console.log("Light:", this.light);
        console.log("Resevoir:", this.resevoir);
        var li = document.createElement("li");
        if (v != "update") {
          li.innerHTML = v;
        } else {
          li.innerHTML = "Watering Plant";
        }
        var list = document.querySelector("#received-messages");
        list.prepend(li);
      });
    },
    padZero(num) {
      return num < 10 ? `0${num}` : num;
    }
  },
  created() {
    // Additional initialization logic if needed
  }
}).mount("#app");

