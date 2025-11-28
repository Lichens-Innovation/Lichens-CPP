/*
 * Created on Mon Nov 24 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */
#pragma once

#include <functional>

namespace LichensCPP
{
    class ScopeAction
    {
    public:
        typedef std::function<void()> ActionType;

        template <typename Func>
        explicit ScopeAction(Func &&action)
            : action_(std::forward<Func>(action))
        {
        }

        ScopeAction(const ScopeAction &) = delete;
        ScopeAction &operator=(const ScopeAction &) = delete;

        ~ScopeAction()
        {
            if (action_ != nullptr)
            {
                action_();
            }
        }

    private:
        ActionType action_;
    };

} // namespace LichensCPP
