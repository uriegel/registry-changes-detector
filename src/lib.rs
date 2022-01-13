use neon::prelude::*;
use std::ptr::null_mut;
use winapi:: {
    shared::minwindef::{HKEY, DWORD},
    um::winreg::{RegOpenKeyW, HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER, RegQueryValueExW, RegCloseKey, RegNotifyChangeKeyValue}
};

#[neon::main]
fn main(mut cx: ModuleContext) -> NeonResult<()> {
    cx.export_function("register", register)?;
    cx.export_function("getDWord", get_dword)?;
    cx.export_function("unregister", unregister)?;
    Ok(())
}

fn register(mut cx: FunctionContext) -> JsResult<JsNumber> {
    let key = cx.argument::<JsString>(0)?.value(&mut cx);
    let pos = key.find('\\').unwrap_or(0);
    let root = &key[0..pos];
    let keystr = to_wstring(&key[pos+1..]);
    let root = match root {
        "HKEY_CURRENT_USER" => HKEY_CURRENT_USER,
        _ => HKEY_LOCAL_MACHINE
    };
    let mut hkey: HKEY = 0 as HKEY;

    let result = unsafe {
        let _res = RegOpenKeyW(root, keystr.as_ptr(), &mut hkey);
        hkey as u32
    };
    
    Ok(cx.number(result))
}

fn get_dword(mut cx: FunctionContext) -> JsResult<JsNumber> {
    let key = cx.argument::<JsNumber>(0)?.value(&mut cx) as u32 as HKEY;
    let value = cx.argument::<JsString>(1)?.value(&mut cx);
    let value_ptr = to_wstring(&value);
    let def_value = cx.argument::<JsNumber>(2)?.value(&mut cx) as u32;
    let mut buffer = vec![6u8; 4];
    let mut size: DWORD = 4;
    let val = unsafe {
        if RegQueryValueExW(key, value_ptr.as_ptr(), null_mut(), null_mut(), buffer.as_mut_ptr(), &mut size) == 0 {
            let mut dst = [0u8; 4];
            dst.clone_from_slice(&buffer[0..4]);
            u32::from_le_bytes(dst)
        } else { def_value }
    };
    Ok(cx.number(val))
    
    
}

fn unregister(mut cx: FunctionContext) -> JsResult<JsUndefined> {
    let key = cx.argument::<JsNumber>(0)?.value(&mut cx) as u32 as HKEY;
    if key as u32 != 0 { 
        unsafe { let _res = RegCloseKey(key); }
//        RegNotifyChangeKeyValue()
    }
    Ok(cx.undefined())
}

// Get win32 lpstr from &str, converting u8 to u16 and appending '\0'
// See retep998's traits for a more general solution: https://users.rust-lang.org/t/tidy-pattern-to-work-with-lpstr-mutable-char-array/2976/2
pub fn to_wstring(value: &str) -> Vec<u16> {
    use std::os::windows::ffi::OsStrExt;

    std::ffi::OsStr::new(value)
        .encode_wide()
        .chain(std::iter::once(0))
        .collect()
}

