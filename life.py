
import numpy as np
from argparse import ArgumentParser
from scipy.ndimage import convolve
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
import matplotlib.animation as animation
from numba import njit, vectorize


def conway_life_conv(size, iteations, world=None, cmap=['white', 'black'], figsize=(11,11), save=False, save_path=None):
    """ Generates conways game of live based on size, iteration parameters using a 3x3 convolutional kernel. """
    if world is None:
        world  = np.random.randint(0, 2, (size,size))
        world[size//2, size//2-1:size//2+2] = 1
    kernel = np.ones((3,3))
    kernel[1,1] = 0

    images = []
    fig = plt.figure(figsize=figsize)
    plt.xticks([])
    plt.yticks([])
    plt.tight_layout()
    for _ in range(iteations):
        new_world = convolve(world, kernel, mode='constant')
        alive = (new_world+world != new_world)
        dead = ~alive
        new_world[alive] = np.logical_and(new_world[alive] >= 2, new_world[alive] <= 3).astype(int)
        new_world[dead] = (new_world[dead] == 3).astype(int)
        # add updated world to images
        images.append([plt.imshow(new_world, animated=True, cmap=ListedColormap(cmap))])
        world = new_world
    
    ani = animation.ArtistAnimation(fig, images, interval=40, blit=True, repeat_delay=100)

    if save and (save_path is not None):
        Writer = animation.writers['ffmpeg']
        writer = Writer(fps=10, metadata=dict(artist='Me'), bitrate=-1)
        ani.save(save_path, writer=writer, dpi=200)
    else:
        plt.show()




# %%
if __name__ == '__main__':
    conway_life_conv(500, 2000, save=True, save_path='conway_life.mp4')