import tkinter as tk
from tkinter import messagebox
from datetime import datetime

def calculate():
    in_time = in_entry.get()
    out_time = out_entry.get()
    price = price_entry.get()

    try:
        in_time = datetime.strptime(in_time, "%H:%M")
        out_time = datetime.strptime(out_time, "%H:%M")
        price = float(price)
    except ValueError:
        messagebox.showerror("Invalid input", "Please enter valid time and price")
        return

    if out_time <= in_time:
        messagebox.showerror("Invalid input", "Out time must be after In time")
        return

    time_diff = (out_time - in_time).seconds / 3600
    total_value = time_diff * price
    result_text.delete("1.0", tk.END)
    result_text.insert(tk.END, f"Out - In = {time_diff:.2f} hours * {price:.2f} = {total_value:.2f}")

# Create the main window
root = tk.Tk()
root.title("Employer Application")
root.configure(bg='yellow')
root.geometry("400x300")

# Create and place the widgets
tk.Label(root, text="In (hh:mm):", bg='yellow').grid(row=0, column=0, padx=10, pady=5)
in_entry = tk.Entry(root)
in_entry.grid(row=0, column=1, padx=10, pady=5)

tk.Label(root, text="Out (hh:mm):", bg='yellow').grid(row=1, column=0, padx=10, pady=5)
out_entry = tk.Entry(root)
out_entry.grid(row=1, column=1, padx=10, pady=5)

tk.Label(root, text="Price:", bg='yellow').grid(row=2, column=0, padx=10, pady=5)
price_entry = tk.Entry(root)
price_entry.grid(row=2, column=1, padx=10, pady=5)

calc_button = tk.Button(root, text="=", command=calculate)
calc_button.grid(row=3, column=0, columnspan=2, pady=10)

result_text = tk.Text(root, width=40, height=10)
result_text.grid(row=4, column=0, columnspan=2, padx=10, pady=10)

# Run the application
root.mainloop()

