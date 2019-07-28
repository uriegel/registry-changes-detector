const addon = require('bindings')('registry-changes-detector')
const addon2 = require('node-addon-api')
console.log(addon2.include)

const eventSource = addon.register({
        key: "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        value: "SystemUsesLightTheme",
        type: "number"
    }, n => {
    console.log(`from callback: ${n}`)    
})

const value = addon.getDWord(eventSource)
console.log(`initial: ${value}`)    

setTimeout(() => {
    console.log("Time is out!")
    addon.unregister(eventSource)
}, 30000)


