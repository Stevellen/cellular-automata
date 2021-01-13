using Plots;


bool(x::Int) = (x!=0); # helper function since Julia doesn't like to use ints as bools
int(x::Float64) = floor(Int, x); # converts floats to int8
int(xs::Array{Int8,1}) = sum([x<<(length(xs)-i) for (i,x) in enumerate(xs)])


plot(grid) = heatmap(1:size(grid,2), 1:size(grid,1),
                reverse(grid, dims=1), c = :greys, axis=([], false),
                legend=false, size=(1920, 1080))



function eca(N::Int, it::Int, rule::Int, k::Int=1)
    mask = 1
    world = zeros(Int8, it, N)  # Create grid
    world[1,int(N/2)] = 1;      # start with a single one in the middle        
    
    # main update loop
    for i in 1:size(world, 1)-1
        for j in 1:size(world, 2)
            b = mod1((j-k+N), N)
            e = mod1((j+k+N), N)
            if b < e
                val = int(world[i,b:e])
            else
                val = int(vcat(world[i, b:N], world[i, 1:e]))
                if bool(rule & (mask<<val))
                end
            end
            if bool(rule & (mask<<val))
                world[i+1, j] = 1
            end
        end
    end
    return world;
end


function k_color(N::Int, it::Int, rule::Int, k::Int=3) # Creates 
    world = zeros(Int8, it, N)  # Create grid
    world[1,div(N,2)] = 1;      # start with a single one in the middle        
    rule_basek = reverse([parse(Int8, c) for c in string(rule, base=k)]) 
    
    # main update loop
    for i in 1:size(world, 1)-1
        for j in 1:size(world, 2)
            b = mod1((j-1+N), N)
            e = mod1((j+1+N), N)
            if b < e
                val = sum(world[i,b:e])
            else
                val = sum([world[i,b], world[i,j], world[i,e]])
            end
            world[i+1, j] = rule_basek[val+1]
        end
    end
    return world; 
end