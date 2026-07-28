#!/usr/bin/env python3
# coding=utf-8

"""
IZV cast2 projektu 2025
Autor: Smirnov Nikita (xsmirn02)
"""

#For prettier type in function
from typing import Dict

from matplotlib import pyplot as plt
import pandas as pd
import seaborn as sns
import numpy as np
import zipfile

# muzete pridat libovolnou zakladni knihovnu ci knihovnu predstavenou na prednaskach
# dalsi knihovny pak na dotaz

#MAPPINGS
REGION_MAP = {
    0: "PHA", 1: "STC", 2: "JHC", 3: "PLK", 4: "ULK",
    5: "HKK",6:"JHM", 7: "MSK", 14: "OLK", 15: "ZLK",
    16: "VYS", 17: "PAK", 18:"LBK", 19: "KVK"
}

RSTAV_MAP = {
    3: "léky/narkotik",
    4: "alkohol do 0.99‰",
    5: "alkohol 1‰ a více",
    6: "nemoc, úraz apod.",
    7: "invalida",
    8: "řidič při jízdě zemřel",
    9: "sebevražda/pokus"
}

CONSEQ_MAP = {
    4: "bez zranění",
    3: "lehké zranění",
    2: "těžké zranění",
    1: "usmrcení"
}

ALC_MAP = {
    0: "nezjišťováno",
    1: "alkohol do 0.24‰",
    2: "ne (bez alkoholu)",
    3: "alkohol 0.24-0.5‰",
    4: "odmítnuto",
    6: "alkohol 0.5-0.8‰",
    7: "alkohol 0.8-1.0‰",
    8: "alkohol 1.0-1.5‰",
    9: "alkohol 1.5‰+"
}


#UTILITY
def _read_xls(zf : zipfile.ZipFile, name : str) -> pd.DataFrame:
    """Reads first table from xls(html) in zip file
    in cp1250 encoding and returns DataFrame

    Args:
        zf (zipfile.ZipFile): Zip file to read from
        name (str): name of file in zip

    Returns:
        pd.DataFrame: DataFrame of first table in file
    """
    raw = zf.read(name)
    table = pd.read_html(raw, encoding="cp1250")
    if not table:
        print(f"No table in {name}")
        return None

    # Assume that we extracted only one table
    dframe = table[0]
    # Get rid of Unnamed column
    dframe = dframe.loc[:, ~dframe.columns.str.contains('^Unnamed')]
    return dframe


def _format_int_range(df_target: pd.DataFrame,
                      index: str, min_r: int = None, max_r: int = None,
                      new_column: str = None, mapping: Dict[int, str] = None) -> pd.DataFrame:
    """Help function that supports formating for [int] columns
    in DataFrame. Without optional params converts all values in column
    to numeric. 
    
    If [min_r] or [max_r] are specified, removes all values under or above limits
    
    If [new_column] and [mapping] are specified, creates new column and fills
    values according to [mapping]

    Args:
        df_target (pd.DataFrame): DataFrame to modify
        index (str): index to work on
        min_r (int, optional): Cuts all values under <min_r>. Defaults to None.
        max_r (int, optional): Cuts all values abovr <max_r>. Defaults to None.
        new_column (str, optional): Name of new column if mapping presented. Defaults to None.
        mapping (Dict[int, str], optional): Mapping for new column. Defaults to None.

    Returns:
        pd.DataFrame: Formated copy of DataFrame
    """
    dframe = df_target.copy()
    dframe[index] = pd.to_numeric(dframe[index], errors="coerce")

    # Create a copy after filtering to avoid SettingWithCopyWarning
    # Not the fastest way, but more applicable
    if min_r is not None:
        dframe = dframe[(dframe[index] >= min_r)].copy()
    if max_r is not None:
        dframe = dframe[(dframe[index] <= max_r)].copy()

    if new_column and mapping:
        dframe[new_column] = dframe[index].map(mapping)

    return dframe


def _save_show_figure(figure, fig_location: str = None,
                      show_figure: bool = False):
    """Help function that displays or saves figure

    Args:
        figure (_type_): figure to work with
        fig_location (str, optional): If specified, saves figure. Defaults to None.
        show_figure (bool, optional): If specified, displays figure. Defaults to False.
    """
    if fig_location:
        figure.savefig(fig_location, bbox_inches='tight')
    if show_figure:
        plt.show(block=True)
    plt.close(figure)

# Ukol 1: nacteni dat ze ZIP souboru
def load_data(filename : str, ds : str) -> pd.DataFrame:
    """Loads all I<ds>.xls files from Zip, supports loading
    from multiple tables with the same name 
    presented in Zip(must be in different files)

    Args:
        filename (str): path to zip file with data
        ds (str): name of desired data file

    Returns:
        pd.DataFrame: Concatenated DataFrame from all files
    """
    target = f"I{ds}.xls"
    dfs = []
    with zipfile.ZipFile(filename, 'r') as zf:
        if not zf:
            print(f"{ds} not found")
            return None
        for name in zf.namelist():
            if name.endswith(target):
                dframe = _read_xls(zf, name)
                dfs.append(dframe)

    if not dfs:
        print(f"No data for {ds}")
        return None

    result = pd.concat(dfs, ignore_index=True)
    return result

# Ukol 2: zpracovani dat
def parse_data(df: pd.DataFrame, verbose: bool = False) -> pd.DataFrame:
    """Preparing data for analysis
    - If [p2a] is present, converts data in column to datetime(%d%m%Y)
    - If [p4a] is present, converts data in column to region mapping
    - If [p1] has duplicates, then drops others then first

    Args:
        df (pd.DataFrame): table to prepare
        verbose (bool, optional): Outputs new size of DataFrame. Defaults to False.

    Returns:
        pd.DataFrame: Formatted DataFrame
    """
    result = df.copy()

    if "p2a" in result.columns:
        result["p2a"] = pd.to_datetime(result["p2a"], errors="coerce", dayfirst=True)

    if "p4a" in result.columns:
        result = _format_int_range(result, "p4a", new_column="region", mapping=REGION_MAP)

    if "p1" in result.columns:
        result = result.drop_duplicates(subset=["p1"], keep="first")

    if verbose:
        size_b = result.memory_usage(deep=True).sum()
        size_mb = size_b / 1000000.0
        print(f"new_size={size_mb} MB")

    return result

# Ukol 3: počty nehod v jednotlivých regionech podle stavu řidiče
def plot_state(df: pd.DataFrame, df_vehicles : pd.DataFrame, fig_location: str = None,
                    show_figure: bool = False):
    """Plot counts of accidents by region for up to 6 most
    frequent driver states

    Args:
        df (pd.DataFrame): DataFrame of accidents
        df_vehicles (pd.DataFrame): DataFrame of vehicles
        fig_location (str, optional): If specified, saves figure. Defaults to None.
        show_figure (bool, optional): If specified, displays figure. Defaults to False.
    """
    vehicles = df_vehicles[["p1", "p57"]].copy()
    vehicles = _format_int_range(vehicles, "p57", 3, 9, "r_stav", RSTAV_MAP)

    neh_vehicles = pd.merge(df[["p1", "region"]], vehicles, on="p1")

    # We have 7 condition types and only 6 graphs, so lets take top 6
    condition_types = neh_vehicles["r_stav"].value_counts().index[:6]

    # Build graph
    fig, axes = plt.subplots(3, 2, figsize=(10, 8))
    axes = axes.ravel()

    for i, (ax, state) in enumerate(zip(axes, condition_types)):
        sub = neh_vehicles[neh_vehicles["r_stav"] == state]
        counts = sub.groupby("region").size().sort_values(ascending=False)

        sns.barplot(x=counts.index, y=counts.values, ax=ax)

        ax.set_title(f"Stav řidiče: {state}", fontsize=14)
        ax.set_xlabel("Kraj" if (i >= 4) else "", fontsize=14)
        ax.set_ylabel("Počet nehod" if (i % 2 == 0) else "", fontsize=14)

        ax.tick_params(axis='x', labelsize=9, labelrotation=30)
        ax.tick_params(axis='y', labelsize=9)

    fig.tight_layout()
    _save_show_figure(fig, fig_location, show_figure)

# Ukol4: alkohol a roky v krajích
def plot_alcohol(df: pd.DataFrame, df_consequences : pd.DataFrame, 
                 fig_location: str = None, show_figure: bool = False):
    """Plot counts of accidents, if alcohol is present, by region and year 
    for 4 types of consequences   

    Args:
        df (pd.DataFrame): DataFrame of accidents 
        df_consequences (pd.DataFrame): DataFrame of consequences
        fig_location (str, optional): If specified, saves figure. Defaults to None.
        show_figure (bool, optional): If specified, displays figure. Defaults to False.
    """
    nehody = df[["p1", "p2a", "region", "p11"]].copy()
    neh_conseq = pd.merge(nehody, df_consequences, on="p1")

    neh_conseq = _format_int_range(neh_conseq, "p11", min_r=3)
    neh_conseq = _format_int_range(neh_conseq, "p59g", 1, 4, "injury", CONSEQ_MAP)

    # Find last month in last year and filter out monthes past this
    last_year = neh_conseq['p2a'].dt.year.max()
    df_last_year = neh_conseq[neh_conseq['p2a'].dt.year == last_year]
    max_month = df_last_year['p2a'].dt.month.max()
    neh_conseq = neh_conseq[neh_conseq['p2a'].dt.month <= max_month]

    neh_conseq["year"] = neh_conseq["p2a"].dt.year
    group = neh_conseq.groupby(["injury", "region", "year"]).size().reset_index(name="count")

    # One for legend
    fig = plt.figure(figsize=(10, 8))
    subfigs = fig.subfigures(1, 2, width_ratios=[10, 1])

    axes = subfigs[0].subplots(2, 2)
    axes = axes.ravel()

    for i, (ax, injury_type) in enumerate(zip(axes, CONSEQ_MAP.values())):
        sub = group[group["injury"] == injury_type]

        sns.barplot(data=sub, x="region", y="count", hue="year", ax=ax)

        ax.set_title(f"Druh zranění: {injury_type}", fontsize=14)
        ax.set_xlabel("Kraj" if (i >= 2) else "", fontsize=14)
        ax.set_ylabel("Počet nehod" if (i % 2 == 0) else "", fontsize=14)

        ax.tick_params(axis="x", labelsize=9, labelrotation=30)
        ax.tick_params(axis="y", labelsize=9)

    # Very complicated way of displaying legend
    handles, labels = ax.get_legend_handles_labels()
    [ax.legend_.remove() for ax in axes]

    legend_fig = subfigs[1]
    legend_ax = legend_fig.add_subplot(111)
    legend_ax.axis('off')

    legend_ax.legend(handles, labels, title="Rok",
           loc="center left", fontsize=9, title_fontsize=14)

    plt.tight_layout()
    _save_show_figure(fig, fig_location, show_figure)


# Ukol 5: Podmínky v čase
def plot_conditions(df: pd.DataFrame, fig_location: str = None,
                    show_figure: bool = False):
    """Plot dynamic of accidents by amount of alcohol present, for
    4 selected regions (top by accidents). In period from 01.01.2023 to 01.01.2025

    Args:
        df (pd.DataFrame): DataFrame of accidents
        fig_location (str, optional): If specified, saves figure. Defaults to None.
        show_figure (bool, optional): If specified, displays figure. Defaults to False.
    """
    nehody = df[["p1", "p2a", "region", "p11"]].copy()
    nehody = _format_int_range(nehody[nehody["p11"] != 5], "p11", new_column="alc", mapping=ALC_MAP)

    # Get top 4 alcohol conditions
    top4 = nehody["region"].value_counts().index[:4]
    nehody = nehody[nehody["region"].isin(top4)]

    # One for legend
    fig = plt.figure(figsize=(10, 6))
    subfigs = fig.subfigures(1, 2, width_ratios=[6, 1])

    axes = subfigs[0].subplots(2, 2)
    axes = axes.ravel()

    for i, (ax, reg) in enumerate(zip(axes, top4)):
        sub = nehody[nehody["region"] == reg]

        # Some melting magic for data collecting
        table_r = (
            sub
            .groupby(["p2a", "alc"])
            .size()
            .reset_index(name="count")
            .pivot_table(index="p2a", columns="alc", values="count", fill_value=0)
            .resample("ME").sum()
        )

        melted_r = (
            table_r
            .reset_index()
            .melt(id_vars="p2a", var_name="alc", value_name="count")
        )

        sns.lineplot(data=melted_r, x="p2a", y="count", hue="alc", ax=ax)

        # set lables
        ax.set_title(f"Kraj: {reg}", fontsize=14)
        ax.set_ylabel("Počet nehod" if (i % 2 == 0) else "", fontsize=14)
        ax.set_xlabel("")

        # Make strict period and display data in nice format
        ax.set_xlim(pd.Timestamp("2023-01-01"), pd.Timestamp("2025-01-01"))
        ax.xaxis.set_major_formatter(plt.matplotlib.dates.DateFormatter('%m/%y'))
        ax.tick_params(axis="x", labelsize=9)
        ax.tick_params(axis="y", labelsize=9)

    # Very complicated way of displaying legend
    handles, labels = ax.get_legend_handles_labels()
    [ax.legend_.remove() for ax in axes]

    legend_fig = subfigs[1]
    legend_ax = legend_fig.add_subplot(111)
    legend_ax.axis('off')

    legend_ax.legend(handles, labels, title="Podminky",
           loc="center left", fontsize=9, title_fontsize=14)

    plt.tight_layout()
    _save_show_figure(fig, fig_location, show_figure)


if __name__ == "__main__":
    # zde je ukazka pouziti, tuto cast muzete modifikovat podle libosti
    # skript nebude pri testovani pousten primo, ale budou volany konkreni
    # funkce.

    df = load_data("data_23_25.zip", "nehody")
    df_consequences = load_data("data_23_25.zip", "nasledky")
    df_vehicles = load_data("data_23_25.zip", "Vozidla")
    df2 = parse_data(df, True)

    plot_state(df2, df_vehicles, "01_state.png")
    plot_alcohol(df2, df_consequences, "02_alcohol.png")
    plot_conditions(df2, "03_conditions.png")

# Poznamka:
# pro to, abyste se vyhnuli castemu nacitani muzete vyuzit napr
# VS Code a oznaceni jako bunky (radek #%%% )
# Pak muzete data jednou nacist a dale ladit jednotlive funkce
# Pripadne si muzete vysledny dataframe ulozit nekam na disk (pro ladici
# ucely) a nacitat jej naparsovany z disku
