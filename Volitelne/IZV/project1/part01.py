#!/usr/bin/env python3
"""
IZV cast1 projektu 2025
Autor: Smirnov Nikita (xsmirn02)

Detailni zadani projektu je v samostatnem projektu e-learningu.
Nezapomente na to, ze python soubory maji dane formatovani.

Muzete pouzit libovolnou vestavenou knihovnu a knihovny predstavene
na prednasce
"""
from bs4 import BeautifulSoup
import requests
import numpy as np
from numpy.typing import NDArray
import matplotlib.pyplot as plt
from typing import List, Dict, Any


def wave_inference_bad(
    x: NDArray[any], y: NDArray[any], sources: NDArray[any], wavelength: float
) -> NDArray[any]:
    """
    Referencni implementace, ktera je pomala a nevyuziva numpy efektivne;
    nezasahujte do ni!
    """
    k = 2 * np.pi / wavelength

    Z = np.zeros(x.shape + y.shape)
    for sx, sy in sources:
        for i in range(x.shape[0]):
            for j in range(y.shape[0]):
                R = np.sqrt((x[i] - sx) ** 2 + (y[j] - sy) ** 2)
                Z[j, i] += np.cos(k * R) / (1 + R)
    return Z


def wave_inference(
    x: NDArray[any], y: NDArray[any], sources: NDArray[any], wavelength: float
) -> NDArray[any]:
    k = 2 * np.pi / wavelength

    X, Y = np.meshgrid(x, y)

    dx = X[..., None] - sources[:, 0]
    dy = Y[..., None] - sources[:, 1]

    R = np.sqrt(dx**2 + dy**2)
    Z = np.sum(np.cos(k * R) / (1 + R), -1)

    return Z


def plot_wave(
    Z: NDArray[any],
    x: NDArray[any],
    y: NDArray[any],
    show_figure: bool = False,
    save_path: str | None = None,
):
    plt.figure(figsize=(6,4))
    plt.imshow(
        Z, 
        extent=(x.min(), x.max(), y.min(), y.max()), 
        vmax=1, 
        vmin=-1
    )
    plt.colorbar(label="Amplituda vlny")
    plt.xlabel("X pozice")
    plt.ylabel("Y pozice")
    plt.title("Vlnové pole")

    if save_path: #Pokud se volá po plt.show(), uloží se prázdný obrázek.
        plt.savefig(save_path)
    if show_figure:
        plt.show()
    plt.close()


def generate_sinus(show_figure: bool = False, save_path: str | None = None):
    x = np.linspace(0,4*np.pi, 200)
    sinx = np.sin(x)
    cosx = np.cos(x)

    fig, axes = plt.subplots(
        ncols=1, nrows=2, figsize=(8,7), sharex=True, sharey=True
    )

    # First plot
    axes[0].plot(x, sinx, color="grey") 
    axes[0].plot(x, cosx, color="grey")
    axes[0].fill_between(x, sinx, cosx, color="green", alpha=0.3)

    # Second plot
    min_vals = np.minimum(sinx, cosx)
    max_vals = np.maximum(sinx, cosx)
    sin_mask = sinx > cosx
    cos_mask = ~sin_mask

    y_blue = np.where(sin_mask, max_vals, np.nan)
    y_orange = np.where(cos_mask, max_vals, np.nan)
    axes[1].plot(x, y_blue, color='blue')
    axes[1].plot(x, y_orange, color='orange')

    #min
    axes[1].plot(x, min_vals, linestyle='--', color='grey')

    #formating x axis
    axes[0].xaxis.set_major_locator(plt.MultipleLocator(np.pi / 2))
    axes[0].xaxis.set_major_formatter(plt.FuncFormatter(format_x_axis))

    for axis in axes:
        axis.set_ylabel("f(x)")

        axis.spines["top"].set_visible(False)
        axis.spines["right"].set_visible(False)
        
        axis.set_ylim(-1.5, 1.5)
        axis.set_xlim(0, np.pi * 4)

        axis.grid(axis="both", linewidth=.5, linestyle="--")

    if save_path:
        plt.savefig(save_path)
    if show_figure:
        plt.show()
    plt.close(fig)

def format_x_axis(value, tick_number):
    N = int(np.round(2 * value / np.pi))
    if N>=0 and N < 3:
        return {0: "0", 1: r"$\frac{1}{2}\pi$", 2: r"$\pi$"}[N]
    elif N % 2 > 0:
        return fr"$\frac{{{N}}}{{2}}\pi$"
    else:
        return fr"${N // 2}\pi$"


def download_data() -> Dict[str, List[Any]]:
    # url = "https://ehw.fit.vutbr.cz/izv/stanice.html" 
    # This page uses 
    # loadstatic( 'st_zemepis_cz.html', '#loadedcontent', {"hrefs": false, "inutf8": false} );
    # so I guess we can use https://ehw.fit.vutbr.cz/izv/st_zemepis_cz.html 
    # Of course we can extract this url from <script> with bs4 but I think this is unnecessary

    url = "https://ehw.fit.vutbr.cz/izv/st_zemepis_cz.html"
    response = requests.get(url)
    soup = BeautifulSoup(response.content, "html.parser")

    rows = soup.find_all('tr')[2:]

    positions: List[str] = []
    lats: List[float] = []
    longs: List[float] = []
    heights: List[float] = []

    for row in rows:
        cols = [c.get_text() for c in row.find_all("td")]

        positions.append(cols[0])
        lats.append(float(cols[2][:-1].replace(",", ".")))
        longs.append(float(cols[4][:-1].replace(",", ".")))
        heights.append(float(cols[6].replace("\xa0", "").replace(",", ".")))

    return {
        "positions": positions, 
        "lats": lats, 
        "longs": longs, 
        "heights": heights,
    }

# def adjust_

if __name__ == "__main__":
    X = np.linspace(-10, 10, 200)
    Y = np.linspace(-10, 10, 200)
    A = wave_inference(X, Y, np.array([[-3, 0], [3, 0], [0, 4]]), 2)
    # plot_wave(A, X, Y, show_figure=True, save_path="tmp_wave.png")
    generate_sinus(show_figure=True, save_path="tmp_sin.png")
    # download_data()
