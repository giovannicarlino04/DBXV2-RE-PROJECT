import xml.etree.ElementTree as ET
import tkinter as tk
from tkinter import ttk
from tkinter import ttk
from ttkthemes import ThemedStyle
import subprocess


serializer_path = 'XV2 XML Serializer.exe'
psc_file = './parameter_spec_char.psc'
xml_file = './parameter_spec_char.psc.xml'

command = [serializer_path, psc_file]
command2 = [serializer_path, xml_file]

try:
    subprocess.run(command, check=True)
except FileNotFoundError:
    print(f"Error: {serializer_path} not found.")
except subprocess.CalledProcessError as e:
    print(f"Command '{command}' returned non-zero exit status {e.returncode}.")

def load_xml(file_path):
    tree = ET.parse(file_path)
    root = tree.getroot()
    return root

def load_configurations(xml_root):
    configurations = []
    for config in xml_root.findall('./Configuration'):
        config_index = config.get('Index')
        configurations.append(config_index)
    return configurations

def load_psc_entries(xml_root):
    psc_entries = []
    for psc_entry in xml_root.findall('./Configuration/PSC_Entry'):
        chara_id = psc_entry.get('Chara_ID')
        psc_entries.append(chara_id)
    return psc_entries

def load_psc_spec_entries(xml_root, selected_config, selected_psc_entry):
    psc_spec_entries = []
    psc_entry = xml_root.find(f"./Configuration[@Index='{selected_config}']/PSC_Entry[@Chara_ID='{selected_psc_entry}']")
    if psc_entry is not None:
        for psc_spec_entry in psc_entry.findall('PscSpecEntry'):
            costume = psc_spec_entry.get('Costume')
            costume2 = psc_spec_entry.get('Costume2')
            if costume:
                psc_spec_entries.append(costume)
            if costume2:
                psc_spec_entries.append(costume2)
    return psc_spec_entries

def show_values(event):
    selected_config = config_combobox.get()
    selected_psc_entry = psc_entry_combobox.get()
    selected_psc_spec_entry = psc_spec_entry_combobox.get()
    
    for entry in root.findall(f"./Configuration[@Index='{selected_config}']/PSC_Entry[@Chara_ID='{selected_psc_entry}']/PscSpecEntry"):
        if entry.get('Costume') == selected_psc_spec_entry or entry.get('Costume2') == selected_psc_spec_entry:
            for child in entry:
                if child.tag in values_entries:
                    if 'value' in child.attrib:
                        values_entries[child.tag].delete(0, tk.END)
                        values_entries[child.tag].insert(0, child.get('value'))
                    elif 'talisman' in child.attrib:  # Check for 'talisman' attribute
                        values_entries[child.tag].delete(0, tk.END)
                        values_entries[child.tag].insert(0, child.get('talisman'))
                    else:
                        # If both 'value' and 'talisman' are missing, handle it here
                        pass  # Add your handling logic or set a default value
            break

root = load_xml('parameter_spec_char.psc.xml')
configurations = load_configurations(root)
psc_entries = load_psc_entries(root)

def save_xml(file_path, root):
    tree = ET.ElementTree(root)
    with open(file_path, "wb") as file:
        tree.write(file)

def save_values():
    selected_config = config_combobox.get()
    selected_psc_entry = psc_entry_combobox.get()
    selected_psc_spec_entry = psc_spec_entry_combobox.get()

    for entry in root.findall(f"./Configuration[@Index='{selected_config}']/PSC_Entry[@Chara_ID='{selected_psc_entry}']/PscSpecEntry"):
        if entry.get('Costume') == selected_psc_spec_entry or entry.get('Costume2') == selected_psc_spec_entry:
            for child in entry:
                if child.tag in values_entries:
                    child.set('value', values_entries[child.tag].get())

    save_xml('parameter_spec_char.psc.xml', root)
    try:
        subprocess.run(command2, check=True)
    except FileNotFoundError:
        print(f"Error: {serializer_path} not found.")
    except subprocess.CalledProcessError as e:
        print(f"Command '{command2}' returned non-zero exit status {e.returncode}.")


window = tk.Tk()
window.title("YaPSCEditor")

config_label = tk.Label(window, text="Select Configuration:")
config_label.grid(row=0, column=0, padx=5, pady=5, sticky='w')

config_combobox = ttk.Combobox(window, values=configurations, width=20)
config_combobox.grid(row=0, column=1, padx=5, pady=5, sticky='w')
config_combobox.bind("<<ComboboxSelected>>", show_values)

psc_entry_label = tk.Label(window, text="Select PSC Entry:")
psc_entry_label.grid(row=1, column=0, padx=5, pady=5, sticky='w')

psc_entry_combobox = ttk.Combobox(window, values=psc_entries, width=20)
psc_entry_combobox.grid(row=1, column=1, padx=5, pady=5, sticky='w')

values_entries = {}

psc_spec_entry_label = tk.Label(window, text="Select PSC Spec Entry:")
psc_spec_entry_label.grid(row=2, column=0, padx=5, pady=5, sticky='w')

psc_spec_entry_combobox = ttk.Combobox(window, values=[], width=20)
psc_spec_entry_combobox.grid(row=2, column=1, padx=5, pady=5, sticky='w')

save_button = ttk.Button(window, text="Save", command=save_values)
save_button.grid(row=0, column=2, padx=5, pady=5, sticky='w')

def update_psc_spec_entries(event):
    selected_config = config_combobox.get()
    selected_psc_entry = psc_entry_combobox.get()
    psc_spec_entries = load_psc_spec_entries(root, selected_config, selected_psc_entry)
    psc_spec_entry_combobox['values'] = psc_spec_entries
    psc_spec_entry_combobox.bind("<<ComboboxSelected>>", show_values)


psc_entry_combobox.bind("<<ComboboxSelected>>", update_psc_spec_entries)

attributes = [
    "Camera_Position", "I_12", "I_16", "Health", "F_24", "Ki", "Ki_Recharge", "I_36", "I_40",
    "I_44", "Stamina", "Stamina_Recharge_Move", "Stamina_Recharge_Air", "Stamina_Recharge_Ground",
    "Stamina_Drain_Rate_1", "Stamina_Drain_Rate_2", "F_72", "Basic_Atk", "Basic_Ki_Atk",
    "Strike_Atk", "Super_Ki_Blast_Atk", "Basic_Atk_Defense", "Basic_Ki_Atk_Defense",
    "Strike_Atk_Defense", "Super_Ki_Blast_Defense", "Ground_Speed", "Air_Speed", "Boosting_Speed",
    "Dash_Speed", "F_124", "Reinforcement_Skill_Duration", "F_132", "Revival_HP_Amount", "F_140",
    "Reviving_Speed", "I_148", "I_152", "I_156", "I_160", "I_164", "I_168", "I_172", "I_176",
    "Super_Soul", "I_184", "I_188", "F_192", "NEW_I_20"
]


canvas = tk.Canvas(window)
canvas.grid(row=3, column=0, columnspan=3, padx=5, pady=5, sticky='nsew')

scrollbar = ttk.Scrollbar(window, orient="vertical", command=canvas.yview)
scrollbar.grid(row=3, column=3, sticky='ns')
canvas.configure(yscrollcommand=scrollbar.set)

frame = tk.Frame(canvas)
canvas.create_window((0, 0), window=frame, anchor='nw')

config_entry_row = 0
for child in attributes:
    label = tk.Label(frame, text=child + ":", anchor='e', width=15)
    label.grid(row=config_entry_row, column=0, padx=5, pady=2, sticky='w')

    entry_field = tk.Entry(frame, width=20)
    entry_field.grid(row=config_entry_row, column=1, padx=5, pady=2, sticky='w')

    values_entries[child] = entry_field
    config_entry_row += 1

def configure_scroll_region(event):
    canvas.configure(scrollregion=canvas.bbox("all"))

frame.bind("<Configure>", configure_scroll_region)

window.update_idletasks()
canvas.config(width=300, height=300) 

def set_theme(theme_name):
    style = ThemedStyle()
    style.set_theme(theme_name)

set_theme("plastik")

window.mainloop()
