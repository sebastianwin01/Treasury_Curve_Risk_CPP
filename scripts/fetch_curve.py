''' Script goes into FRED, downloads history of each Treasury Yield and
    deletes empty observations. Keeping in it the most recent data      '''

import csv                  # Read csv files
import urllib.request       # Connects to URL and downloads data
import io                   # Uses text in memory as a file

series = {                  # Dictionary - FRED
    "2Y": "DGS2",
    "5Y": "DGS5",
    "10Y": "DGS10",
    "30Y": "DGS30"
}

for maturity, fred_id in series.items():                                        # Matches maturity and ID

    url = f"https://fred.stlouisfed.org/graph/fredgraph.csv?id={fred_id}"

    with urllib.request.urlopen(url) as response:           # Connects to that address
        data = response.read().decode("utf-8")              # Convert read bytes into string

    rows = list(csv.DictReader(io.StringIO(data)))          # Text convertion into rows

    valid_rows = [                              # Storing only valid data
        row for row in rows
        if row[fred_id] not in ("", ".")
    ]

    latest = valid_rows[-1]                     # Last valid observation

    print(
        maturity,
        latest["observation_date"],
        latest[fred_id]
    )