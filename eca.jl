using Plots;
pyplot();
theme(:juno)


bool(x::Int) = (x!=0); # helper function since Julia doesn't like to use ints as bools
int(x::Float64) = floor(Int, x); # converts floats to int8
int(x,y,z) = (x<<2 | y<<1 | z); # converts 3 values into the integer represented by their bitstring

plot(grid) = heatmap(1:size(grid,1), 1:size(grid,2),
                grid', c = :greys, size=(1920, 900))

function eca(N, it, rule)
    mask = 1
    world = zeros(Int8, it, N)  # Create grid
    world[1,int(N/2)] = 1;      # start with a single zero in the middle        
    
    # main update loop
    for i in 1:size(world, 1)-1
        for j in 1:size(world, 2)
            b = mod1((j-1+N), N)
            e = mod1((j+1+N), N)
            if b < e
                val = int(world[i,b:e]...)
            else
                val = int(world[i,b], world[i,j], world[i,e])
            end
            if bool(rule & (mask<<val))
                world[i+1, j] = 1
            end
        end
    end
    return world;
end
