use neon::prelude::*;

#[neon::main]
fn main(mut cx: ModuleContext) -> NeonResult<()> {
    cx.export_function("register", register)?;
    Ok(())
}

fn register(mut cx: FunctionContext) -> JsResult<JsNumber> {
    let key = cx.argument::<JsString>(0)?.value(&mut cx);
    let value = cx.argument::<JsString>(1)?.value(&mut cx);
    // RegOpenKey
    Ok(cx.number(23))
}

// fn getEvent(mut cx: FunctionContext) -> JsResult<JsNumber> {
    // callback function
// }

// fn unregister(mut cx: FunctionContext) -> JsResult<JsUndefined> {
//     // RegCloseKey
//     Ok(cx.undefined())
// }
