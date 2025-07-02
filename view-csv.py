import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


def analyze_markov_chain(csv_file='markov_chain_results.csv'):
    # Read the CSV file
    df = pd.read_csv(csv_file)

    # Convert state strings to numerical values (1 for active, 0 for inactive)
    df['state_before_num'] = df['state_before'].map({'active': 1, 'inactive': 0})
    df['state_after_num'] = df['state_after'].map({'active': 1, 'inactive': 0})

    # Calculate transition probabilities
    transitions = df.groupby(['state_before', 'state_after']).size().unstack()

    # Estimate delta0 (active -> inactive)
    active_total = transitions.loc['active'].sum()
    active_to_inactive = transitions.loc['active', 'inactive'] if 'inactive' in transitions.loc['active'] else 0
    delta0_est = active_to_inactive / active_total if active_total > 0 else 0

    # Estimate delta1 (inactive -> active)
    inactive_total = transitions.loc['inactive'].sum()
    inactive_to_active = transitions.loc['inactive', 'active'] if 'active' in transitions.loc['inactive'] else 0
    delta1_est = inactive_to_active / inactive_total if inactive_total > 0 else 0

    # Plot the state transitions
    plt.figure(figsize=(12, 6))
    plt.step(df['step'], df['state_before_num'], where='post', label='State Before')
    # plt.step(df['step'], df['state_after_num'], where='post', label='State After', linestyle='--')
    plt.yticks([0, 1], ['Inactive', 'Active'])
    plt.xlabel('Step')
    plt.ylabel('State')
    plt.title('Markov Chain State Transitions')

    plt.gcf().text(0.1, 0.9, f"delta0 (active → inactive): {delta0_est:.4f}")
    plt.gcf().text(0.1, 0.95, f"delta1 (inactive → active): {delta1_est:.4f}")

    plt.legend()
    plt.grid(True)
    plt.show()

    print("\nEstimated Transition Probabilities:")
    print(f"delta0 (active → inactive): {delta0_est:.4f}")
    print(f"delta1 (inactive → active): {delta1_est:.4f}")

    return delta0_est, delta1_est


if __name__ == "__main__":
    delta0_est, delta1_est = analyze_markov_chain()