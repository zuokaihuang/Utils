--require "fire_not_exist"
-- 不要直接require 出错脚本就会停止运行
local status, module = pcall(require, 'fire')
fire_module = status and module or nil
print("rs==>", status, fire_module)

status, module = pcall(require, "bundle")
bundle_module = status and module or nil

function sayhello()
    print ("hello hello");
    return 0;
end

function printme( a, b, c, d, e)
    print("[lua]print me:", a, b, c, d, e)
    return 0;
end

function main()
    print ("now run in main");
    printme (package.path);
    sayhello();
    if fire_module ~= nil then fire() end
    if bundle_module ~= nil then walk() end
    -- not_exist();
    return 0;
end

main();

--foo();


gtable={
    name="helloy",
    path="ppp",
    num=123,
    fb={name="world", path="inner", num=124},
    tb={name="world", path="inner", num=124},
    fptr=sayhello,
    inner_func=function() print("inner_func:", gtable.data_in_c_inner ) end
}

function printc()
    print("printc:", data_in_c);
    -- we can not see {data_in_c_inner}
    print("printc:", gtable.data_in_c_inner);
end

