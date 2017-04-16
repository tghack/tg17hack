function display(loc){
    var newQuery = "?lat=" + loc.coords.latitude + "&lon=" + loc.coords.longitude;
    if(newQuery != location.search){
        location.search = newQuery;
    }
}
if(navigator.geolocation){
    navigator.geolocation.getCurrentPosition(display);
}
