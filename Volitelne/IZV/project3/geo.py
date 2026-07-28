#!/usr/bin/python3.13
# coding=utf-8
"""
IZV cast 3.1 projektu 2025
Autor: Smirnov Nikita (xsmirn02)
"""
# %%%
import pandas as pd
import geopandas
import matplotlib.pyplot as plt
import contextily as ctx
import sklearn.cluster
import numpy as np
from mpl_toolkits.axes_grid1 import make_axes_locatable

# %%%
def _save_show_figure(figure, fig_location: str = None,
                      show_figure: bool = False):
    """Help function that displays or saves figure

    Args:
        figure (_type_): figure to work with
        fig_location (str, optional): If specified, saves figure. Defaults to None.
        show_figure (bool, optional): If specified, displays figure. Defaults to False.
    """
    plt.tight_layout()

    if fig_location:
        figure.savefig(fig_location, bbox_inches='tight')
    if show_figure:
        plt.show(block=True)
    plt.close(figure)

# %%%
def make_geo(
    df_accidents: pd.DataFrame,
    df_locations: pd.DataFrame
    ) -> geopandas.GeoDataFrame:
    """Creates GeoDataFrame from input df's with accidents and locations.

    Args:
        df_accidents (pd.DataFrame): DataFrame with accidents inforamtion.
        df_locations (pd.DataFrame): DataFrame with locations information.

    Returns:
        geopandas.GeoDataFrame: GeoDataFrame with formatted and merged information.
    """

    df = df_accidents.merge(df_locations, on='p1', how='inner')

    # Remove all dates with bad formatting
    df = df[(df['d'] != 0) & (df['e'] != 0)].copy()

    mask = df['d'] < df['e']
    df.loc[mask, ['d', 'e']] = df.loc[mask, ['e', 'd']].values

    # GeoDataFrame creating with 'Křovákovo zobrazení'
    gdf = geopandas.GeoDataFrame(
        df,
        geometry=geopandas.points_from_xy(df['d'], df['e']),
        crs='EPSG:5514'
    )

    return gdf

# %%%
def plot_geo(
    gdf: geopandas.GeoDataFrame,
    fig_location: str = None,
    show_figure: bool = False
    ):
    """Process information about accidents happened in STC region 
    where animals was involved.

    Args:
        gdf (geopandas.GeoDataFrame): GeoDataFrame with filtered data from make_geo().
        fig_location (str, optional): Path for plot saving. Defaults to None.
        show_figure (bool, optional): Show plot after processing. Defaults to False.
    """
    # We are working with STC region
    gdf_stc = gdf[(gdf['region'] == 'STC') & (gdf['p10'] == 4)].copy()

    # Extract year from date
    gdf_stc['year'] = pd.to_datetime(gdf_stc['p2a'], dayfirst=True).dt.year

    # Conversion to Web Mercator for contextily
    gdf_wm = gdf_stc.to_crs('EPSG:3857')

    gdf_2023 = gdf_wm[gdf_wm['year'] == 2023]
    gdf_2024 = gdf_wm[gdf_wm['year'] == 2024]

    fig, axes = plt.subplots(1, 2, figsize=(14, 8))

    # Plots
    for gdf_year, ax, year in zip(
        [gdf_2023, gdf_2024],
        axes,
        [2023, 2024]
    ):
        gdf_year.plot(
            ax=ax,
            markersize=3,
            color='red',
            alpha=0.5,
        )
        ctx.add_basemap(
            ax,
            crs=gdf_2023.crs.to_string(),
            source=ctx.providers.OpenStreetMap.Mapnik,
        )

        ax.set_title(f'Nehody se zvěří - STC kraj {year}', fontsize=14)
        ax.set_aspect('equal')
        ax.axis('off')

    _save_show_figure(fig, fig_location, show_figure)

# %%%
def _kmeans_cluster(coords: np.array):
    # The most commonly used variant and also the fastest for this task.
    # Due to those factors, and also due to a good result after clustering 
    # we would use this method.
    # The main weakness of this method - you should decide the number of clusters 
    # by yourself. Numbers lower than 10 - results in "not enough detailed" regions
    # numbers bigger than 20 - creates too many separate regions unnecessary for our
    # task. So, I think 17 suits the best for analysis for STC region.
    kmeans = sklearn.cluster.KMeans(
        n_clusters=17,
        random_state=0,
        n_init=10
    )

    return kmeans.fit_predict(coords)

def _dbscan_cluster(coords: np.array):
    # Always tries to create one big cluster, and after that carve
    # the most independent clusters inside
    # after trying different values for different parametrs, Its pretty hard to
    # obtain good and small clusters with big density.
    # Usually its either one big cluster or many small ones.
    dbscan = sklearn.cluster.DBSCAN(
        eps=11000,
        min_samples=50
    )

    return dbscan.fit_predict(coords)

def _agglomerative_cluster(coords: np.array):
    # Slower than kmeans but results in very similar clusters
    # also here appears connectivity, that we actually dont need
    agg = sklearn.cluster.AgglomerativeClustering(
        n_clusters=17
    )

    return agg.fit_predict(coords)

def _affinity_cluster(coords: np.array):
    # very slow, but looks not bad in some situations
    # results depends on initial state, sometimes creates a lot of small ones.
    aff = sklearn.cluster.AffinityPropagation(
        random_state=1,
        # convergence_iter=4
    )

    return aff.fit_predict(coords)

def _meanshift_cluster(coords: np.array):
    # Even slower than affinity propagation, but results in more consistent results.
    # Sometimes, mostly to {cluster_all=True} creates weirdly streched clusters
    ms = sklearn.cluster.MeanShift(
        # bin_seeding=True,
        bandwidth=17000,
    )

    return ms.fit_predict(coords)

def plot_cluster(
    gdf: geopandas.GeoDataFrame,
    fig_location: str = None,
    show_figure: bool = False):
    """Plots the accidents under the influence of alcohol, grouped in clusters.

    Args:
        gdf (geopandas.GeoDataFrame): GeoDataFrame with filtered data from make_geo().
        fig_location (str, optional): Path for plot saving. Defaults to None.
        show_figure (bool, optional): Show plot after processing. Defaults to False.
    """
    gdf_stc = gdf[(gdf['region'] == 'STC') & (gdf['p11'] >= 4)].copy()

    # Conversion to Web Mercator for contextily
    gdf_wm = gdf_stc.to_crs('EPSG:3857')

    coords = np.array([[point.x, point.y] for point in gdf_wm.geometry])

    # K-means method with 17 clusters.
    # Read comments in corresponding function for details.
    gdf_wm['cluster'] = _kmeans_cluster(coords)

    # count number of accidents per cluster
    cluster_counts = gdf_wm.groupby('cluster').size()

    # Plot the result
    fig, ax = plt.subplots(1, 1, figsize=(14, 8))

    # for color bar
    divider = make_axes_locatable(ax)
    cax = divider.append_axes('right', size='5%', pad=0.1)

    # Dissolving for proper cluster visualisation and coloring
    gdf_clusters = gdf_wm.dissolve(by='cluster', aggfunc='first')
    gdf_clusters['count'] = cluster_counts

    gdf_clusters.convex_hull.to_frame('geometry').assign(
        count=gdf_clusters['count'].values
    ).plot(
        ax=ax,
        cax=cax,
        column='count',
        alpha=0.4,
        edgecolor='black',
        linewidth=1.5,
        cmap='YlOrRd',
        vmin=cluster_counts.min(),
        vmax=cluster_counts.max(),
        legend=True
    )

    # plots accidents as red dots on top of clusters
    gdf_wm.plot(
        ax=ax,
        markersize=3,
        color='red',
        alpha=0.5,
        zorder=2
    )
    ctx.add_basemap(
        ax,
        crs=gdf_wm.crs.to_string(),
        source=ctx.providers.OpenStreetMap.Mapnik,
    )

    # Sets the limits for displayed map
    # In the task was mentioned:
    #   if we have accident with bad region assigned, just set correct map limits
    # therefor I just incresed a liitle, the value of gdf_wm.total_bounds[0]
    # howewer, cluster still "contains" that wrong point inside
    ax.set_xlim(1489232, gdf_wm.total_bounds[2])
    ax.set_ylim(gdf_wm.total_bounds[1], gdf_wm.total_bounds[3])
    cax.set_ylabel('Počet nehod v úseku')

    ax.set_title('Nehody v STC kraji s významnou měrou alkoholu', fontsize=14)
    ax.set_aspect('equal')
    ax.axis('off')

    _save_show_figure(fig, fig_location, show_figure)

# %%%
if __name__ == "__main__":
    # %%% First Test
    t_df_accidents = pd.read_pickle("accidents.pkl.gz")
    t_df_locations = pd.read_pickle("locations.pkl.gz")
    t_gdf = make_geo(t_df_accidents, t_df_locations)

    print(f"Number of extracted records: {len(t_gdf)}/{len(t_df_accidents)}")
    print(f"CRS: {t_gdf.crs}")
    print(t_gdf.head())

    # %%% Second test
    plot_geo(t_gdf, "geo1.png", True)

    # %%% Third test
    plot_cluster(t_gdf, "geo2.png", True)

    # %%% testovani splneni zadani
    import os
    assert os.path.exists("geo1.png")
    assert os.path.exists("geo2.png")
