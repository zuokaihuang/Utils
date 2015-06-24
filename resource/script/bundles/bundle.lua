all_bundle={
    {name='box', module="box/main"},
    {name='player', module="player/main"}
}

function bundle_update()
    print("bundle update");
    require("box/main")
    box.update();

    for k,v in pairs(all_bundle) do
        module = v['module']
        name    = v['name']
        print( k, "==>", v, "==>", module , "==>", name);
        require(module)
    end

    return 0;
end

