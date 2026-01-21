/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chsauvag <chsauvag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:01:15 by chsauvag          #+#    #+#             */
/*   Updated: 2026/01/21 16:07:08 by chsauvag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"
/*Common Nickname Rules & Invalid Characters:
Allowed Characters: Letters (A-Z, a-z), numbers (0-9), hyphens (-), and specific symbols often include []{}^|_`.
Disallowed Characters: Spaces, NUL (\x00), BELL (\x07), Line Feed (\x0A), Carriage Return (\x0D), and the colon : are typically forbidden.
First Character: Must be a letter or one of the allowed special characters; names starting with numbers (like 4chan) or certain symbols (like ///) can be invalid.
Length: While RFCs suggest a limit (e.g., 9 characters), many networks allow longer nicks (e.g., 32 characters).
Uniqueness: Nicknames must generally be unique on the network; using someone else's (or a reserved service name) can lead to disconnects. 
Why Nicknames Become Invalid:
Protocol Violations: Using forbidden characters breaks the IRC protocol.
Network Limits: Exceeding the network's maximum nickname length.
Security/Abuse: Using names reserved for network staff or common names (like "Admin").*/