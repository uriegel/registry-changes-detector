const { register, getDWord, unregister, detectChange } = require("./index.node")

function registerAddon(params, cb) {
    let handle = register(params.key, params.value)
    params["handle"] = handle
    params["recentVal"] = getDWordAddon(params, 0)
    detectChangeAddon(params, cb)
    return params
}

function getDWordAddon(params, defValue) {
    return getDWord(params.handle, params.value, defValue || 0)
}

function unregisterAddon(params) {
    params.recentVal = 0
    unregister(params.handle)
}

async function detectChangeAddon(params, cb) {
    while (true) {
        if (await detectChangeAsync(params.handle) == false)
            break
        let newVal = getDWordAddon(params, 0)
        if (newVal != params.recentVal) {
            params.recentVal = newVal
            cb(newVal)    
        }
    }
}

const detectChangeAsync = handle => new Promise(res => detectChange(handle, res))

exports.register = registerAddon
exports.getDWord = getDWordAddon
exports.unregister = unregisterAddon