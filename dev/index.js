const addon = require('bindings')('registry-changes-detector')

const eventSource = addon.register({
        key: "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        value: "SystemUsesLightTheme",
        type: "number"
    }, n => {
    console.log(`from callback: ${n}`)    
})

setTimeout(() => {
    console.log("Time is out!")
    addon.unregister(eventSource)
}, 30000)


