const addon = require('bindings')('registry-changes-detector')

const eventSource = addon.register(n => {
    console.log(`from callback: ${n}`)    
})

setTimeout(() => addon.unregister(eventSource), 20000)
console.log('Event Source')

