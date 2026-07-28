#!/usr/bin/python3.13
# coding=utf-8
"""
IZV cast 3.3 projektu 2025
Autor: Smirnov Nikita (xsmirn02)
"""

# %%%
import pandas as pd
import matplotlib.pyplot as plt
# import numpy as np
# import seaborn as sns

# %%%
def prepare_data(df: pd.DataFrame) -> pd.DataFrame:
    """Prepares accident data, by adding binary columns for easier computations

    Args:
        df (pd.DataFrame): dataframe with accident records

    Returns:
        pd.DataFrame: prepared dataframe
    """
    df_acc = df.copy()

    # Extract date components
    df_acc['month'] = df_acc['date'].dt.month
    df_acc['year'] = df_acc['date'].dt.year

    # Define complicating factors
    # Alcohol >0.24‰
    ALC_CODES = [3, 6, 7, 8, 9]
    # Drugs
    DRG_CODES = [1, 2, 3, 4, 5, 6]
    # Road states [wet, mud, black ice, oil, snow, other]
    ROD_CODES = [3, 4, 6, 7, 8, 9]
    # Weather [fog, rain, snow, balack ice, wind]
    WTH_CODES = [2, 4, 5, 6, 7]
    # Poor visibility
    VSB_CODES = [2, 3, 5, 7]

    # Sort out complicating factors
    df_acc['alcohol'] = df_acc['p11'].isin(ALC_CODES).astype(int)
    df_acc['drugs'] = df_acc['p11a'].isin(DRG_CODES).astype(int)
    df_acc['road_state'] = df_acc['p16'].isin(ROD_CODES).astype(int)
    df_acc['bad_weather'] = df_acc['p18'].isin(WTH_CODES).astype(int)
    df_acc['poor_visibility'] = df_acc['p19'].isin(VSB_CODES).astype(int)

    # Any complication (binary: 0 or 1 per accident)
    df_acc['has_complication'] = (
        (df_acc['alcohol'] == 1) |
        (df_acc['drugs'] == 1) |
        (df_acc['poor_visibility'] == 1) |
        (df_acc['bad_weather'] == 1) |
        (df_acc['road_state'] == 1)
    ).astype(int)

    return df_acc

# %%%
def assign_season(month: int) -> str:
    """Assign season based on month.

    Args:
        month (int): month[1, 12]

    Returns:
        str: Season name
    """
    if month in [12, 1, 2]:
        return 'Winter'
    elif month in [3, 4, 5]:
        return 'Spring'
    elif month in [6, 7, 8]:
        return 'Summer'
    else:  # 9, 10, 11
        return 'Autumn'

# %%%
def generate_plot(df_prep: pd.DataFrame):
    """Generate stacked bar chart with average monthly accidents.
    Separeted accidents where any of complications were present.

    Args:
        df_prep (pd.DataFrame): prepared dataframe of accidents
    """
    # We already know from previous task that data in 2025 ends somwhere in october
    # the best method in this case, manually get rid of them, in larger datasets
    # we can get rid of the next line and that should work fine.
    # We can visually see strange deviations on plot on small dataset.
    df_prep = df_prep[
        ~((df_prep['year'] == 2025) & (df_prep['month'] >= 10))
    ]

    # Count actual occurrences of each month in the dataset
    monthly = (df_prep.groupby(['year', 'month', 'has_complication'])
               .size().reset_index(name='count'))

    # Average over years for each month (works properly with absent monthes
    # but didnt work if only part of month data is persistent)
    monthly_avg = (monthly.groupby(['month', 'has_complication'])['count']
        .mean().unstack(fill_value=0))

    months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun',
              'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']

    _, ax = plt.subplots(figsize=(12, 6))

    # W/O complicating factors
    ax.bar(months, monthly_avg[0], color='lightblue', label='No complications')
    # W complicating factors
    ax.bar(months, monthly_avg[1], color='red', label='With complications',
                   bottom=monthly_avg[0])

    # Mean values for both? Looks weird on stacked bar, todo later
    # ax.axhline(y=avg_no_compl.mean(), color='blue', linestyle='--', alpha=0.7)
    # ax.axhline(y=avg_compl.mean(), color='red', linestyle='--', alpha=0.7)

    # Fancy stuff
    ax.set_ylabel('Avg. number of accidents per month', fontsize=12, fontweight='bold')
    ax.set_title('Average monthly road accidents: impact of complicating factors',
                 fontsize=14, fontweight='bold')
    ax.legend(fontsize=11)
    ax.grid(axis='y', alpha=0.4)

    plt.tight_layout()
    plt.savefig('fig.pdf', format='pdf', dpi=300, bbox_inches='tight')
    plt.close()

# %%%
def generate_table(df_prep: pd.DataFrame) -> pd.DataFrame:
    """Generate seasonal statistics table.

    Args:
        df_prep (pd.DataFrame): prepared dataframe of accidents

    Returns:
        pd.DataFrame: table that contain complicating factors per season
    """
    df = df_prep.copy()
    df['season'] = df['month'].apply(assign_season)

    season_order = ['Winter', 'Spring', 'Summer', 'Autumn']

    # Aggregate by season
    agg = (df.groupby('season').agg(
            total=('p1', 'count'),
            has_complication=('has_complication', 'sum'),
            alcohol=('alcohol', 'sum'),
            drugs=('drugs', 'sum'),
            poor_visibility=('poor_visibility', 'sum'),
            bad_weather=('bad_weather', 'sum'),
            road_cond=('road_state', 'sum')
        ).reindex(season_order))

    agg['no_compl'] = agg['total'] - agg['has_complication']
    agg['compl_pct'] = (agg['has_complication'] / agg['total'] * 100).round(2)

    # Prepare output table
    table = agg.reset_index()[[
        'season',
        'total',
        'no_compl',
        'alcohol',
        'drugs',
        'poor_visibility',
        'bad_weather',
        'road_cond',
        'compl_pct'
    ]]

    table.columns = [
        'Season', 'Total', 'No comp.', 'Alcohol', 'Drugs',
        'Visibility', 'Weather', 'Road cond.', 'Comp. rate'
    ]

    print("\nSeasonal breakdown of complicating factors")
    print(table.to_latex(
        index=False,
        float_format="%.2f",
        caption="Seasonal breakdown of complicating factors",
        label="tab:seasonal_complications"
    ))

    return table

# %%%
def calculate_metrics(s_df: pd.DataFrame):
    """Calculate and print key analytical metrics based on seasonal table

    Args:
        s_df (pd.DataFrame): seasonal table
    """

    print("\nCalculated key metrics for the report\n")

    # 1. Accident number by season (Min vs Max)
    # And increase % calculation
    max_row = s_df.loc[s_df['Total'].idxmax()]
    min_row = s_df.loc[s_df['Total'].idxmin()]

    growth_pct = (max_row['Total'] / min_row['Total'] - 1) * 100

    print("1) Accident volume by season:")
    print(f"\tMost accidents : {max_row['Season']} ({int(max_row['Total'])})")
    print(f"\tLeast accidents: {min_row['Season']} ({int(min_row['Total'])})")
    print(f"\tRelative increase: {growth_pct:.2f}%\n")

    # 2. Complicated accidents: Winter vs Summer(previous calculated min/max)
    # And difference % calculation
    winter = s_df[s_df['Season'] == 'Winter'].iloc[0]
    summer = s_df[s_df['Season'] == 'Summer'].iloc[0]

    winter_rate = winter['Comp. rate']
    summer_rate = summer['Comp. rate']

    difference = (winter_rate / summer_rate - 1) * 100

    print("2) Complication % comparison (Winter vs Summer):")
    print(f"\tWinter: {int(winter['Total'])} accidents, {winter_rate:.2f}% with complications")
    print(f"\tSummer: {int(summer['Total'])} accidents, {summer_rate:.2f}% with complications")
    print(f"\tRelative increase in winter: {difference:.2f}%\n")

    # 3. Absolute number of complicated accidents
    # Not the best metric for the task, but we woud need this later
    print("3) Absolute number of complicated accidents:")
    print(f"\tWinter: {int(winter['Total'] - winter['No comp.'])}")
    print(f"\tSummer: {int(summer['Total'] - summer['No comp.'])}\n")

    # 4. % of presented factors in complicated accidents
    factors = ['Alcohol', 'Drugs', 'Visibility', 'Weather', 'Road cond.']

    print("4) % of complicating factors presented by season")
    print("\tShare of each factor among complicated accidents:")

    for season_row in [winter, summer]:
        compl_total = season_row['Total'] - season_row['No comp.']
        print(f"\n\t{season_row['Season']}:")

        for f in factors:
            share = season_row[f] / compl_total * 100
            print(f"\t {f:11s}: {share:.2f}%")

# %%%
if __name__ == "__main__":
    df_accidents = pd.read_pickle("accidents.pkl.gz")
    df_prep_accidents = prepare_data(df_accidents)

    print(f"Loaded {len(df_prep_accidents)} accident records")

    # Generate outputs
    generate_plot(df_prep_accidents)
    seasonal = generate_table(df_prep_accidents)
    calculate_metrics(seasonal)

    print("\nAnalysis complete")
