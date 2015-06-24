require "walk"

function fire()
    walk();
    print ("fire");
    return 0;
end

function fire_inner()
    print (" fire inner function")
end

fire_inner();

