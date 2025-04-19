import ROOT
import pandas as pd

# Load ROOT file and trees
f = ROOT.TFile("g4beamline.root")
vd = f.Get("VirtualDetector/VD")
new = f.Get("NTuple/NEW")
loss = f.Get("NTuple/LOSS")

# Branch names of interest
fields = ["x", "y", "z", "t", "InitKE", "PDGid", "EventID", "TrackID", "ParentID"]

# Load new and loss into separate DataFrames
rows_new = []
rows_loss = []

for i in range(new.GetEntries()):
    new.GetEntry(i)
    row_new = {field: getattr(new, field) for field in fields}
    rows_new.append(row_new)

for i in range(loss.GetEntries()):
    loss.GetEntry(i)
    row_loss = {field: getattr(loss, field) for field in fields}
    rows_loss.append(row_loss)

df_new = pd.DataFrame(rows_new)
df_loss = pd.DataFrame(rows_loss)

# Cast IDs to int for consistency
for df in [df_new, df_loss]:
    df["EventID"] = df["EventID"].astype(int)
    df["TrackID"] = df["TrackID"].astype(int)
    df["ParentID"] = df["ParentID"].astype(int)
    df["PDGid"] = df["PDGid"].astype(int)

# Sort both dataframes by EventID and time
df_new = df_new.sort_values(["EventID", "t"]).reset_index(drop=True)
df_loss = df_loss.sort_values(["EventID", "t"]).reset_index(drop=True)

# Filter df_new to find interesting events
grouped = df_new.groupby("EventID")

def is_interesting(group):
    return not (len(group) == 1 and group["PDGid"].iloc[0] == 2212)

interesting_events = grouped.filter(is_interesting)

# Print result
pd.set_option('display.max_rows', None)
print(interesting_events)

def trace_event(event_id):
    births = df_new[df_new["EventID"] == event_id].copy()
    deaths = df_loss[df_loss["EventID"] == event_id].copy()

    # Rename columns to clarify merge
    births = births.rename(columns={
        "x": "x_birth", "y": "y_birth", "z": "z_birth", "t": "t_birth", "InitKE": "KE_birth"
    })
    deaths = deaths.rename(columns={
        "x": "x_loss", "y": "y_loss", "z": "z_loss", "t": "t_loss", "InitKE": "KE_loss"
    })

    # Merge on TrackID to pair each particle's birth and loss
    merged = pd.merge(births, deaths, on=["EventID", "TrackID"], how="left", suffixes=("", "_loss"))

    # Optional: sort by creation time
    merged = merged.sort_values("t_birth").reset_index(drop=True)

    # Display select columns for readability
    display_cols = [
        "TrackID", "ParentID", "PDGid", "KE_birth",
        "z_birth", "t_birth", "z_loss", "t_loss"
    ]
    print(merged[display_cols])


