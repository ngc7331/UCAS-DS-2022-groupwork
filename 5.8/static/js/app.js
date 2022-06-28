const { createApp } = Vue;

createApp({
    data() {
        return {
            new_city: "",
            new_train: ["",,,,,],
            new_plane: ["",,,,,],
            search_arg: [,,0,0],
            result: {
                interchange: 0,
                trip_duration: 0,
                duration: 0,
                cost: 0,
                path: [],
                tp: 0
            },
            city: {},
            plane: {},
            train: {},
            admin: false,
            algo: "0",
        }
    },
    mounted () {
        let app = this;
        app.loadData(app.city, "data/city.json");
        app.loadData(app.plane, "data/plane.json");
        app.loadData(app.train, "data/train.json");
    },
    computed: {
        new_city_id() {
            return this.newId(this.city);
        },
        new_train_id() {
            return this.newId(this.train);
        },
        new_plane_id() {
            return this.newId(this.plane);
        },
        pathinfo() {
            return this.result.tp == "0" ? this.train : this.plane;
        }
    },
    methods: {
        switchMode: function() {
            if (!this.admin)
                this.copyObject({
                    interchange: 0,
                    trip_duration: 0,
                    duration: 0,
                    cost: 0,
                    path: [],
                    tp: 0
                }, this.result);
            this.admin = !this.admin;
        },
        clearData: function(target) {
            for (var key in target) {
                delete target[key];
            }
        },
        loadData: function(target, url) {
            this.clearData(target);
            console.log("load data from " + url);
            return new Promise((resolve, reject)=>{
                axios.get(url)
                    .then(response => { this.copyObject(response.data, target); })
                    .catch(function (error) { console.log(error); });
            })
        },
        setAlgo: function() {
            axios.get("api/v1/setAlgo?tp=" + this.algo)
                .then(response => {})
                .catch(function (error) { swal({title: "Error", text: error.response.data.err, icon: "error", timer: 2000}); });
        },
        newId: function(obj) {
            var keys = Object.keys(obj);
            if (keys.length == 0) return 0;
            return (parseInt(keys[keys.length-1]) + 1).toString();
        },
        newCity: function() {
            axios.get("api/v1/newCity?name=" + this.new_city)
                .then(response => { this.city[this.new_city_id] = this.new_city; })
                .catch(function (error) { swal({title: "Error", text: error.response.data.err, icon: "error", timer: 2000}); });
        },
        delCity: function(i) {
            let app = this;
            axios.get("api/v1/delCity?id=" + i)
                .then(response => {
                    delete app.city[i];
                    app.loadData(app.plane, "data/plane.json");
                    app.loadData(app.train, "data/train.json");
                })
                .catch(function (error) { swal({title: "Error", text: error.response.data.err, icon: "error", timer: 2000}); });
        },
        newRoute: function(tp) {
            let app = this;
            var d = tp == 0 ? app.new_train : app.new_plane;
            axios.get(`api/v1/newRoute?name=${d[0]}&a=${d[1]}&b=${d[2]}&t=${d[3]}&d=${d[4]}&c=${d[5]}&tp=${tp}`)
                .then(response => {
                    if (tp == 0) {
                        app.loadData(app.train, "data/train.json");
                    } else {
                        app.loadData(app.plane, "data/plane.json");
                    }
                })
                .catch(function (error) { swal({title: "Error", text: error.response.data.err, icon: "error", timer: 2000}); });
        },
        delRoute: function(i, tp) {
            let app = this;
            axios.get("api/v1/delRoute?id=" + i + "&tp=" + tp)
                .then(response => {
                    if (tp == 0) {
                        app.loadData(app.train, "data/train.json");
                    } else {
                        app.loadData(app.plane, "data/plane.json");
                    }
                })
                .catch(function (error) { swal({title: "Error", text: error.response.data.err, icon: "error", timer: 2000}); });
        },
        search: function() {
            let app = this;
            axios.get(`api/v1/search?a=${app.search_arg[0]}&b=${app.search_arg[1]}&tp=${app.search_arg[2]}&policy=${app.search_arg[3]}`)
                .then(response => {
                    app.copyObject(JSON.parse(response.data.data), app.result);
                })
                .catch(function (error) { swal({title: "Error", text: error.response.data.err, icon: "error", timer: 2000}); });
        },
        strfTime: function(t) {
            return Math.floor(t/1440) + "日 " + Math.floor((t%1440)/60).toString().padStart(2,'0') + ":" + (t%60).toString().padStart(2,'0');
        },
        copyObject: function(from, to) {
            for (var key in from) {
                to[key] = from[key];
            }
        }
    }
}).mount('#app');
